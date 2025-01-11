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

   template <ErrorHandling Action = ErrorHandling::Throwing>
   constexpr void expects(bool condition, std::string_view error_message)
   {
      if constexpr (Action == ErrorHandling::Throwing) {
         if (condition == false) {
            throw std::runtime_error(error_message.data());
         }
      }
      if constexpr (Action == ErrorHandling::Terminate) {
         if (condition == false) {
            std::terminate();
         }
      }
   }

   template <ErrorHandling Action = ErrorHandling::Throwing>
   constexpr void expects(std::predicate auto condition, std::string_view error_message)
   {
      expects<Action>(std::invoke(condition), error_message);
   }

   template <ErrorHandling Action = ErrorHandling::Throwing>
   constexpr void expects_to_have_elements(std::ranges::sized_range auto const& seq, std::string_view error_message)
   {
      expects<Action>([&]() { return seq.size() > 0; }, error_message);
   }
} // namespace sec21
