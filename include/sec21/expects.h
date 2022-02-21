#pragma once

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
   constexpr void expects(Condition condition, std::string_view error_message)
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
   };

   //! \todo replace with std::span<T, N>
   template <ErrorHandling Action = ErrorHandling::Throwing, typename Sequence>
   constexpr void expects_to_have_elements(Sequence const& seq, std::string_view error_message)
   {
      expects<Action>([&]() { return size(seq) > 0; }, std::move(error_message));
   };
} // namespace sec21
