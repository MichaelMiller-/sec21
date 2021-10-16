#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace sec21::database
{
   template <typename DatabaseType, typename T>
   struct data_mapper;

   template <typename DatabaseType>
   struct data_mapper<DatabaseType, DatabaseType>
   {
      using value_t = DatabaseType;
      using source_t = DatabaseType;

      [[nodiscard]] static auto map(value_t const& value) noexcept -> value_t {
         return value;
      }
   };

   template <typename DatabaseType>
   struct data_mapper<DatabaseType, boost::uuids::uuid>
   {
      using value_t = DatabaseType;
      using source_t = boost::uuids::uuid;

      [[nodiscard]] static auto map(source_t const& value) -> value_t {
         return boost::uuids::to_string(value);
      }
      [[nodiscard]] static auto map(value_t const& value) -> source_t {
         static boost::uuids::string_generator string_gen;
         return string_gen(value);
      }
   };
}