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
      static_assert(std::is_trivial_v<underlying_t>, "strong_type: T should be a trivial type");
   private:
      underlying_t value{};
   public:

      constexpr strong_type() = default;
      constexpr explicit strong_type(T const& t) noexcept
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

   //! \todo test if this is a good idea
#if 0
   namespace detail
   {
      template <typename T, typename Tag, template<typename> typename... Policies>
      T underlying_type(strong_type<T, Tag, Policies...>);
   } // namespace detail

   template <typename T>
   using underlying_type = decltype(detail::underlying_type(std::declval<std::decay_t<T>>()));
#endif

   template <typename T, typename U>
   struct mixed_addition
   {
      friend constexpr T& operator += (T& lhs, U const& rhs) noexcept
      {
         using lhs_t = typename T::underlying_t;
         using rhs_t = typename U::underlying_t;
         static_assert(std::is_same_v<lhs_t, rhs_t>);

         static_cast<lhs_t&>(lhs) += static_cast<rhs_t const&>(rhs);
         return lhs;
      }
      friend constexpr T operator + (T const& lhs, U const& rhs) noexcept
      {
         using lhs_t = typename T::underlying_t;
         using rhs_t = typename U::underlying_t;
         static_assert(std::is_same_v<lhs_t, rhs_t>);

         return T(static_cast<lhs_t const&>(lhs) + static_cast<rhs_t const&>(rhs));
      }
   };

   template <typename T>
   using addition = mixed_addition<T, T>;


   template <typename T, typename U>
   struct mixed_subtraction
   {
      friend constexpr T& operator -= (T& lhs, U const& rhs) noexcept
      {
         using lhs_t = typename T::underlying_t;
         using rhs_t = typename U::underlying_t;
         static_assert(std::is_same_v<lhs_t, rhs_t>);

         static_cast<lhs_t&>(lhs) -= static_cast<rhs_t const&>(rhs);
         return lhs;
      }
      friend constexpr T operator - (T const& lhs, U const& rhs) noexcept
      {
         using lhs_t = typename T::underlying_t;
         using rhs_t = typename U::underlying_t;
         static_assert(std::is_same_v<lhs_t, rhs_t>);

         return T(static_cast<lhs_t const&>(lhs) - static_cast<rhs_t const&>(rhs));
      }
   };

   template <typename T>
   using subtraction = mixed_subtraction<T, T>;


   template <typename T>
   struct multiplication
   {
      friend constexpr T& operator *= (T& lhs, T const& rhs) noexcept
      {
         using type = typename T::underlying_t;
         static_cast<type&>(lhs) *= static_cast<type const&>(rhs);
         return lhs;
      }
      friend constexpr T operator * (T const& lhs, T const& rhs) noexcept
      {
         using type = typename T::underlying_t;
         return T(static_cast<type const&>(lhs) * static_cast<type const&>(rhs));
      }
   };

   template <typename T>
   struct division
   {
      friend constexpr T& operator /= (T& lhs, T const& rhs) noexcept
      {
         using type = typename T::underlying_t;
         static_cast<type&>(lhs) /= static_cast<type const&>(rhs);
         return lhs;
      }
      friend constexpr T operator / (T const& lhs, T const& rhs) noexcept
      {
         using type = typename T::underlying_t;
         return T(static_cast<type const&>(lhs) / static_cast<type const&>(rhs));
      }
   };

   template <typename T>
   struct increment
   {
      constexpr T& operator ++ () noexcept
      {
         using type = typename T::underlying_t;
         ++static_cast<type&>(static_cast<T&>(*this));
         return static_cast<T&>(*this);
      }
      constexpr T operator ++ (int) noexcept
      {
         auto result = static_cast<T&>(*this);
         ++*this;
         return result;
      }
   };

   template <typename T>
   struct decrement
   {
      constexpr T& operator -- () noexcept
      {
         using type = typename T::underlying_t;
         --static_cast<type&>(static_cast<T&>(*this));
         return static_cast<T&>(*this);
      }
      constexpr T operator -- (int) noexcept
      {
         auto result = static_cast<T&>(*this);
         --*this;
         return result;
      }
   };

   template <typename T>
   struct integer_arithmetic : addition<T>,
                               subtraction<T>,
                               multiplication<T>,
                               division<T>,
                               increment<T>,
                               decrement<T>
   { };

   template <typename T>
   struct compare
   {
      friend constexpr bool operator == (T const& lhs, const T& rhs) noexcept
      {
         using type = typename T::underlying_t;
         return static_cast<type const&>(lhs) == static_cast<type const&>(rhs);
      }
      friend constexpr bool operator != (T const& lhs, const T& rhs) noexcept
      {
         return !(lhs == rhs);
      }
   };

   template <typename T>
   struct print
   {
      template <typename CharT, typename Traits>
      friend auto& operator << (std::basic_ostream<CharT, Traits>& os, T const& t) noexcept
      {
         using type = typename T::underlying_t;
         return os << static_cast<type const&>(t);
      }
   };
}
