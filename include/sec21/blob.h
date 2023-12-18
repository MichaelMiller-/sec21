#pragma once

#include <cstring>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace sec21
{
   class blob
   {
      std::vector<std::byte> memory{};

    public:
      template <typename T>
      explicit blob(T const& value)
      {
         memory.resize(sizeof(T));
         std::memcpy(memory.data(), &value, sizeof(T));
      }

      template <typename T>
      blob& operator=(T const& value)
      {
         memory.resize(sizeof(T));
         std::memcpy(memory.data(), &value, sizeof(T));
         return *this;
      }

      template <typename T>
         requires std::is_default_constructible_v<T>
      [[nodiscard]] operator T() const
      {
         if (memory.size() < sizeof(T)) {
            throw std::runtime_error("invalid blob size");
         }
         T result{};
         std::memcpy(&result, memory.data(), sizeof(T));
         return result;
      }

      [[nodiscard]] auto size() const noexcept { return memory.size(); }
   };
} // namespace sec21