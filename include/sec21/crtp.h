#pragma once

namespace sec21
{
   //! Curiously recurring template pattern (CRTP)
   //! https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
   //
   template <typename T, template<typename> typename Crtp>
   [[deprecated]] struct crtp
   {
      T& self() noexcept { return static_cast<T&>(*this); }

      const T& self() const noexcept { return static_cast<const T&>(*this);  }
   };
}
