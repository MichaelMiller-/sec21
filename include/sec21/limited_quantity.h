#pragma once

#include <format>
#include <limits>
#include <stdexcept>
#include <utility>

namespace sec21
{
   //! \brief Represents a value between two fixed endpoints [Infimum, Supremum]
   template <std::integral T, T Infimum, T Supremum = std::numeric_limits<T>::max()>
   class limited_quantity
   {
      using value_t = T;
      value_t value{Infimum};

    public:
      [[nodiscard]] constexpr limited_quantity() = default;
      [[nodiscard]] constexpr explicit limited_quantity(value_t v) : value{v}
      {
         if (not in_range(v)) {
            throw std::out_of_range(std::format("Given value ({}) is not in range of [{},{}]", v, Infimum, Supremum));
         }
      }

      //! \brief implicit conversion
      [[nodiscard]] constexpr operator auto() const { return value; } // NOLINT(*-explicit-constructor)

      [[nodiscard]] constexpr auto get() const noexcept -> value_t { return value; }

      [[nodiscard]] friend constexpr auto operator<=>(limited_quantity const&, limited_quantity const&) = default;

      [[nodiscard]] static constexpr auto in_range(value_t value) noexcept
      {
         return std::cmp_greater_equal(value, Infimum) and std::cmp_less_equal(value, Supremum);
      }

      [[nodiscard]] static constexpr auto min() noexcept { return Infimum; }
      [[nodiscard]] static constexpr auto max() noexcept { return Supremum; }

      //! \brief pre-increment
      constexpr auto operator++() -> limited_quantity&
      {
         if (not in_range(value + value_t{1})) {
            throw std::out_of_range("Cannot increment above the upper limit");
         }
         ++value;
         return *this;
      }

      //! \brief pre-decrement
      constexpr auto operator--() -> limited_quantity&
      {
         if (not in_range(value - value_t{1})) {
            throw std::out_of_range("Cannot decrement below the lower limit");
         }
         --value;
         return *this;
      }

      friend auto operator<<(std::ostream& out, limited_quantity const& obj) -> auto& { return out << obj.value; }
   };

   template <std::integral T, T Infimum, T Supremum>
   constexpr auto operator+(limited_quantity<T, Infimum, Supremum> const& lhs,
                            limited_quantity<T, Infimum, Supremum> const& rhs)
   {
      return limited_quantity<T, Infimum, Supremum>{lhs.get() + rhs.get()};
   }

   template <std::integral T, T Infimum, T Supremum>
   constexpr auto operator-(limited_quantity<T, Infimum, Supremum> const& lhs,
                            limited_quantity<T, Infimum, Supremum> const& rhs)
   {
      return limited_quantity<T, Infimum, Supremum>{lhs.get() - rhs.get()};
   }

   template <std::integral T, T Infimum, T Supremum>
   constexpr auto operator*(limited_quantity<T, Infimum, Supremum> const& lhs,
                            limited_quantity<T, Infimum, Supremum> const& rhs)
   {
      return limited_quantity<T, Infimum, Supremum>{lhs.get() * rhs.get()};
   }

   template <std::integral T, T Infimum, T Supremum>
   constexpr auto operator/(limited_quantity<T, Infimum, Supremum> const& lhs,
                            limited_quantity<T, Infimum, Supremum> const& rhs)
   {
      return limited_quantity<T, Infimum, Supremum>{lhs.get() / rhs.get()};
   }
} // namespace sec21
