#pragma once

#include <functional>
#ifdef __cpp_concepts
   #include <concepts>
#endif

//! \todo replace with https://en.cppreference.com/w/cpp/experimental/lib_extensions_3
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
      [[no_unique_address]] Callable func;
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
      [[no_unique_address]] Callable func;
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
