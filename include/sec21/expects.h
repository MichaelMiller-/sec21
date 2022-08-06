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
#if __cpp_lib_source_location >= 201907L
            std::stringstream ss;
            ss << "file: " << location.file_name() << "(" << location.line() << ":" << location.column() << ") "
               << std::quoted(location.function_name()) << ": " << error_message;

            throw std::runtime_error(ss.str());
#else
            throw std::runtime_error(error_message.data());
#endif
         }
      }
      if constexpr (Action == ErrorHandling::Terminate) {
         if (condition() == false) {
            std::terminate();
         }
      }
   }

   template <ErrorHandling Action = ErrorHandling::Throwing, typename Sequence>
   constexpr void expects_to_have_elements(Sequence const& seq, std::string_view error_message)
   {
      expects<Action>([&]() { return seq.size() > 0; }, std::move(error_message));
   }

} // namespace sec21
