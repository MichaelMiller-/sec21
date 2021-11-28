#pragma once

#include <nlohmann/json_fwd.hpp>

#include <memory>
#include <string_view>

namespace sec21
{
   class http_connection
   {
      struct impl;
      std::unique_ptr<impl> pimpl;

    public:
      explicit http_connection(std::string_view host);
      // std::unique_ptr needs a destructor, therefore use "rule of fixe"
      ~http_connection();
      http_connection(http_connection const&) = default;
      http_connection(http_connection &&) = default;
      http_connection& operator = (http_connection const&) = default;
      http_connection& operator = (http_connection &&) = default;

      nlohmann::json get(std::string const& endpoint);

      nlohmann::json post(std::string const& endpoint, nlohmann::json const& value);
   };

   //! \brief Single-shot
   //! \param host
   //! \param endpoint
   //! \return query result
   nlohmann::json http_get(std::string const& host, std::string const& endpoint);
} // namespace sec21