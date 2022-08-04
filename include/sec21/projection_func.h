#pragma once

#include <functional>
#include <utility>

namespace sec21
{
   //! \todo will be replaced with std::ranges:: and std::identity{}
   template <typename Function, typename Projection>
   class projection_func
   {
      Function m_function;
      Projection m_projection;

    public:
      projection_func() = delete;

      explicit projection_func(Function function, Projection proj) noexcept
          : m_function{std::move(function)}, m_projection{std::move(proj)}
      {
      }

      template <typename... Args>
      decltype(auto) operator()(Args&&... args) const
      {
         return std::invoke(m_function, std::invoke(m_projection, std::forward<Args>(args))...);
      }
   };

   template <typename Function, typename Projection, typename T>
   class projection
   {
      Function m_function;
      Projection m_projection;
      T m_value;

    public:
      projection() = delete;

      explicit projection(Function function, Projection proj, T t) noexcept
          : m_function{std::move(function)}, m_projection{std::move(proj)}, m_value{std::move(t)}
      {
      }

      template <typename... Args>
      decltype(auto) operator()(Args&&... args) const
      {
         return std::invoke(m_function, std::invoke(m_projection, std::forward<Args>(args))..., m_value);
      }
   };
} // namespace sec21
