#pragma once

namespace sec21
{
   //! \brief Curiously recurring template pattern (CRTP)
   //! \brief https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
   //
   template <typename T, template<typename> typename Crtp>
   struct crtp
   {
      T& self() noexcept { return static_cast<T&>(*this); }
      const T& self() const noexcept { return static_cast<const T&>(*this);  }
   };
}
