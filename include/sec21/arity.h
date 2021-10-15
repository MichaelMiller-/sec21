#pragma once

namespace sec21
{
   namespace detail
   {
      template <auto>
      struct arbitrary_type
      {
         template <typename T>
         operator T &&();

         template <typename T>
         operator T & ();
      };

      template <typename F, auto... Is, typename U = decltype(std::declval<F>()(arbitrary_type<Is>{}...))>
      constexpr auto test_signature(std::index_sequence<Is...>)
      {
         return std::integral_constant<std::size_t, sizeof...(Is)>{};
      }

      template <auto I, typename F>
      constexpr auto arity_impl(int) -> decltype(test_signature<F>(std::make_index_sequence<I>{}))
      {
         return {};
      }

      template <auto I, typename F, typename = std::enable_if_t<(I > 0)>>
      constexpr auto arity_impl(...)
      {
         // try the int overload which will only work, if F takes I-1 arguments. Otherwise this
         // overload will be selected and we'll try it with one element less.
         return arity_impl<I - 1, F>(0);
      }

      template <typename F>
      constexpr auto arity_impl()
      {
         // start checking function signatures with maximum 10 elements
         return arity_impl<10, F>(0);
      }
   }

   //! \brief count the number of arguments of any given function, including generic lambdas
   template <typename Callable>
   constexpr auto arity(Callable) { return detail::arity_impl<std::decay_t<Callable>>(); }

   namespace compile_time_tests
   {
      struct BinaryFunctor {
         template<typename T, typename U>
         void operator()(T, U) {}
      };
      static_assert(arity(BinaryFunctor{}) == 2);

      const auto l0 = [](){};
      const auto l3 = [](auto, auto, auto){};

      static_assert(arity(l0) == 0);
      static_assert(arity(l3) == 3);
   }
}