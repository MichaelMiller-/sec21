#pragma once

#include <type_traits>
#include <utility>

namespace sec21
{
   template <typename T, typename Tag, template<typename> typename... Policies>
   class strong_type : public Policies<strong_type<T, Tag, Policies...>>...
   {
   public:
      using underlying_t = T;
   private:
      underlying_t value{};
   public:
      constexpr strong_type() = default;
      constexpr explicit strong_type(T const& t) noexcept(std::is_nothrow_copy_constructible<T>::value)
         : value{ t }
      {}
      constexpr explicit strong_type(T&& t) noexcept(std::is_nothrow_move_constructible<T>::value)
         : value{ std::move(t) }
      {}

      explicit operator underlying_t& () noexcept
      {
         return value;
      }

      explicit operator underlying_t const& () const noexcept
      {
         return value;
      }

      friend void swap(strong_type& a, strong_type& b) noexcept
      {
         using std::swap;
         swap(static_cast<T&>(a), static_cast<T&>(b));
      }
   };

   template <typename T>
   struct is_strong_type : std::false_type {};

   template <typename T, typename Tag, template<typename> typename... Policies>
   struct is_strong_type<strong_type<T, Tag, Policies...>> : std::true_type {};

   template <typename T>
   inline constexpr bool is_strong_type_v = is_strong_type<T>::value;

#ifdef __cpp_concepts
   template<typename T>
   concept StrongType = is_strong_type_v<T>;
#endif

   namespace detail
   {
      template <typename T, typename Tag, template<typename> typename... Policies>
      T underlying_type(strong_type<T, Tag, Policies...>);
   } // namespace detail

   template <typename T>
   using underlying_type = decltype(detail::underlying_type(std::declval<std::decay_t<T>>()));
}
