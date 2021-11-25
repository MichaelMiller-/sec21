#pragma once

#include "base_logger.h"

namespace sec21::log
{
   template <typename Key, typename Value>
   class faz
   {
      using value_t = std::vector<Value>;
      std::unordered_map<Key, value_t> values;

    public:
      void insert(Key key, Value v) noexcept
      {
         const auto it = values.find(key);
         if (it != end(values)) {
            it->second.push_back(std::move(v));
         } else {
            values[key] = value_t{std::move(v)};
         }
      }

      auto get(Key key) const -> value_t
      {
         value_t result;
         const auto it = values.find(key);
         if (it != end(values)) {
            std::copy(begin(it->second), end(it->second), std::back_inserter(result));
         }
         return result;
      }
   };

   class string_logger : public base_logger
   {
      faz<int, entry> f;

    public:
      void log(entry const&) override;
   };
} // namespace sec21::log