#pragma once

#include <type_traits>
#include <utility>

namespace sec21
{
   //! \brief
   //! \todo Policies must inherits from crtp<>
   template <typename T, typename Tag, template<typename> typename... Policies>
   class strong_type : public Policies<strong_type<T, Tag, Policies...>>...
   {
      T  m_value{};
   public:
      using type = T;

      constexpr explicit strong_type(T const& value) noexcept
         : m_value{ value } 
      {}
      
      constexpr explicit strong_type(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
         : m_value{ std::move(value) }
      {}

      constexpr T& get() noexcept { return m_value; }
      constexpr const T& get() const noexcept { return m_value; }
   };
}
