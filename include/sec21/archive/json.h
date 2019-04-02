#pragma once

#include <sec21/reflection.h>
#include <sec21/serializer.h>

#include <nlohmann/json.hpp>

namespace sec21
{
   namespace archive
   {
      template <typename Stream>
      struct json
      {
         Stream stream;
         nlohmann::json j{};

         template <typename U>
         explicit json(U && u) noexcept : stream{ u }
         {}

         template <typename U>
         void write(std::string_view name, U const& u) {
            j[name.data()] = u;
         }

         template <typename U>
         void read(std::string_view name, U& u) {
            j.at(name.data()).get_to(u);
         }

         void open() {
            stream >> j;
         }

         void flush() {
            stream << std::setw(3) << j;
         }
      };

      template <typename Stream>
      json(Stream&& s) -> json<Stream>;
   }
}
