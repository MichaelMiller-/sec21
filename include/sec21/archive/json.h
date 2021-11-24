#pragma once

#include <sec21/reflection.h>
#include <sec21/serializer.h>

#include <nlohmann/json.hpp>

#include <iomanip>

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
         explicit json(U && u) noexcept : stream{ std::forward<U>(u) }
         {}

         template <typename U, std::enable_if_t<!reflection::is_registered_v<std::decay_t<U>>, int> = 0>
#ifdef __cpp_conditional_explicit
         explicit(!reflection::is_registered_v<std::decay_t<U>>)
#endif
         void write(std::string_view name, U const& u) {
            j[name.data()] = u;
         }

         template <typename U, std::enable_if_t<reflection::is_registered_v<std::decay_t<U>>, int> = 0>
#ifdef __cpp_conditional_explicit
         explicit(reflection::is_registered_v<std::decay_t<U>>)
#endif
         void write(std::string_view name, U const& u) 
         {
            j[name.data()] = nlohmann::json::object();
            //! \todo serialize object into **j[name.data()]**

            std::cout << "obj: " << name.data() << std::endl;
            //serialize(j[name.data()], u);
            //! \todo 
            std::ignore = name;
            std::ignore = u;
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
