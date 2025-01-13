#pragma once

#include <charconv>

#if __cpp_lib_format >= 201907L
#include <format>
#else
#include <sstream>
#endif

namespace sec21::policy
{
   template <typename T>
   // A(0) ... Z(0) ... A1 ... Z1 ... A2 ... Z2 ...
   struct increment_alphabet
   {
      T& operator++() noexcept
      {
         using type_t = typename T::underlying_t;
         static_assert(std::is_same_v<type_t, std::string>);

         auto& result = static_cast<type_t&>(static_cast<T&>(*this));

         if (result[0] == 'Z') {
            const auto tmp = type_t{next(begin(result)), end(result)};
            unsigned long long number{0};
            std::from_chars(tmp.data(), tmp.data() + tmp.size(), number);
#if __cpp_lib_format >= 201907L
            result = std::format("A{}", ++number);
#else
            {
               std::stringstream stream{};
               stream << 'A' << ++number;
               result = stream.str();
            }
#endif
         } else
            ++result[0];
         return static_cast<T&>(*this);
      }

      T operator++(int) noexcept
      {
         auto result = static_cast<T&>(*this);
         ++*this;
         return result;
      }
   };
} // namespace sec21::policy
