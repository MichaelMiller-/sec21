#pragma once

#include "http_connection_export.h"

#include <nlohmann/json_fwd.hpp>

#include <memory>
#include <string_view>

namespace sec21
{
   class HTTP_CONNECTION_EXPORT http_connection
   {
      struct impl;
      std::shared_ptr<impl> pimpl;

    public:
      explicit http_connection(std::string_view host);

      nlohmann::json get(std::string_view endpoint);

      nlohmann::json post(std::string_view endpoint, nlohmann::json value);
   };

   //! \brief Single-shot
   //! \param host
   //! \param endpoint
   //! \return query result
   nlohmann::json HTTP_CONNECTION_EXPORT http_get(std::string_view host, std::string_view endpoint);
} // namespace sec21