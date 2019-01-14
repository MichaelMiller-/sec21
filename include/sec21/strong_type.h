#pragma once

#include <type_traits>
#include <utility>

namespace sec21
{
   //! \brief
   template <typename T, typename Tag, template<typename> typename... Policies>
   class strong_type : public Policies<strong_type<T, Tag, Policies...>>...
   {
      T  m_value{};
   public:
      using type = T;

      static_assert(std::is_trivial_v<type>, "strong_type: T should be a trivial type");

      constexpr explicit strong_type(T const& value) noexcept
         : m_value{ value } 
      {}
      
      constexpr explicit strong_type(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
         : m_value{ std::move(value) }
      {}

      [[nodiscard]] constexpr auto get() const noexcept -> type { return m_value; }
   };
}
