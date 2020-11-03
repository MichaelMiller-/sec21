#pragma once

#include <functional>
#include <concepts>

namespace sec21
{
   //! \brief scope guard for cleanup and other actions that always happen
#ifdef __cpp_concepts
   template <std::invocable Callable>
#else
   template <typename Callable>
#endif
   class scope_guard
   {
      Callable func;
   public:
      scope_guard() = delete;
      scope_guard(scope_guard const&) = delete;
      scope_guard& operator = (scope_guard const&) = delete;

      constexpr explicit scope_guard(Callable&& f) noexcept : func{ f } {}
      ~scope_guard() { func(); }
   };

   class uncaught_exception_detector
   {
      const int current_exceptions_count;
   public:
      uncaught_exception_detector() noexcept : current_exceptions_count{ std::uncaught_exceptions() } {}

      operator bool() const noexcept {
         return std::uncaught_exceptions() > current_exceptions_count;
      }
   };

   //! \brief scope guard for rollback and other failure handling actions
#ifdef __cpp_concepts
   template <std::invocable Callable>
#else
   template <typename Callable>
#endif
   class scope_guard_if_exception
   {
      Callable func;
      uncaught_exception_detector detector;
   public:
      scope_guard_if_exception() = delete;
      scope_guard_if_exception(scope_guard_if_exception const&) = delete;
      scope_guard_if_exception& operator = (scope_guard_if_exception const&) = delete;

      constexpr explicit scope_guard_if_exception(Callable&& f) noexcept : func{ f } {}
      ~scope_guard_if_exception() {
         if (detector) {
            func();
         }
      }
   };
}
