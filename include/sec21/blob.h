#pragma once

#include <cstring>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace sec21
{
   class blob
   {
      // note: std::byte causes a lot more problems, therefore 'char' is the better type
      std::vector<char> memory;
      static_assert(sizeof(char) == 1);

    public:
      template <typename T>
      explicit constexpr blob(T const& value)
      {
         memory.resize(sizeof(T));
         std::memcpy(memory.data(), &value, sizeof(T));
      }

      struct parse
      {
      };

      blob(parse, std::string const& str)
      {
         std::istringstream stream{str};
         stream >> *this;
      }

      template <typename T>
      constexpr auto operator=(T const& value) -> blob&
      {
         memory.resize(sizeof(T));
         std::memcpy(memory.data(), &value, sizeof(T));
         return *this;
      }

      template <typename T>
         requires std::is_default_constructible_v<T>
      [[nodiscard]] explicit constexpr operator T() const
      {
         if (memory.size() < sizeof(T)) {
            throw std::runtime_error("Invalid blob size");
         }
         T result{};
         std::memcpy(&result, memory.data(), sizeof(T));
         return result;
      }

      [[nodiscard]] constexpr auto size() const noexcept { return memory.size(); }

      template <typename T>
      [[nodiscard]] constexpr auto as() const -> T
      {
         return static_cast<T>(*this);
      }

      friend auto operator<<(std::ostream& output, blob const& obj) -> std::ostream&
      {
         output << obj.size();
         using value_t = decltype(obj.memory)::value_type;
         std::copy(begin(obj.memory), end(obj.memory), std::ostream_iterator<value_t>(output));
         return output;
      }

      friend auto operator>>(std::istream& input, blob& obj) -> std::istream&
      {
         decltype(obj.size()) count{};
         input >> count;
         obj.memory.resize(count);
         input.read(obj.memory.data(), static_cast<std::streamsize>(count));
         return input;
      }
   };

   [[nodiscard]] inline auto to_string(blob const& obj)
   {
      std::stringstream stream;
      stream << obj;
      return stream.str();
   }
} // namespace sec21