#pragma once

namespace sec21
{
   //! Curiously recurring template pattern (CRTP)
   //! https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
   //
   template <typename Derived, template<typename> typename Crtp>
   [[deprecated]] struct crtp
   {
      constexpr Derived& derived() noexcept { return static_cast<Derived&>(*this); }

      constexpr Derived const& derived() const noexcept { return static_cast<Derived const&>(*this);  }
   };
}
