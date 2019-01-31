#pragma once

#include <utility>
#include <functional>

namespace sec21
{
   template <typename Function, typename Projection>
   class projection_func
   {
      Function    m_function;
      Projection  m_projection;

   public:
      projection_func() = delete;

      projection_func(Function function, Projection projection) noexcept
         : m_function{ std::move(function) }
         , m_projection{ std::move(projection) }
      {}

      template <typename... Args>
      decltype(auto) operator() (Args &&... args) const 
      {
         return std::invoke(m_function, std::invoke(m_projection, std::forward<Args>(args))...);
      }
   };
}
