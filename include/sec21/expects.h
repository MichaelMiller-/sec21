#pragma once

#if __cpp_lib_source_location >= 201907L
#include <source_location>
#endif
#include <stdexcept>
#include <string_view>

namespace sec21
{
   enum class ErrorHandling {
      // Logging,
      Throwing,
      Terminate
   };

   // example: expects<ErrorHandling::Throwing>([value]{ return value > 23; }, "value has to be greater than 23");

   template <ErrorHandling Action = ErrorHandling::Throwing, typename Condition>
#if __cpp_lib_source_location >= 201907L
   // \todo make use of std::source_location
   constexpr void expects(Condition condition, std::string_view error_message,
                          std::source_location location = std::source_location::current())
#else
   constexpr void expects(Condition condition, std::string_view error_message)
#endif
   {
      if constexpr (Action == ErrorHandling::Throwing) {
         if (condition() == false) {
            throw std::runtime_error(error_message.data());
         }
      }
      if constexpr (Action == ErrorHandling::Terminate) {
         if (condition() == false) {
            std::terminate();
         }
      }
   }

#if __cpp_lib_span >= 202002L
   template <ErrorHandling Action = ErrorHandling::Throwing, typename T, std::size_t N>
   constexpr void expects_to_have_elements(std::span<T, N> seq, std::string_view error_message)
   {
      expects<Action>([&]() { return seq.size() > 0; }, std::move(error_message));
   }
#else
   template <ErrorHandling Action = ErrorHandling::Throwing, typename Sequence>
   constexpr void expects_to_have_elements(Sequence const& seq, std::string_view error_message)
   {
      expects<Action>([&]() { return seq.size() > 0; }, std::move(error_message));
   }
#endif
} // namespace sec21
