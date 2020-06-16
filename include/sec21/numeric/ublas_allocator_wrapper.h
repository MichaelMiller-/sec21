#pragma once

#include <memory>

namespace sec21::numeric
{
   //! \todo boost::numeric::ublas has problem with std::allocator deprecation 
   template <typename T>
   struct ublas_allocator_wrapper
   {
      T obj{};

      using difference_type = std::allocator_traits<T>::difference_type;
      using size_type = std::allocator_traits<T>::size_type;
      using iterator = std::allocator_traits<T>::pointer;
      using const_iterator = std::allocator_traits<T>::const_pointer;

      template <typename Size>
      [[nodiscard]] constexpr auto allocate(Size n)
      {
         return obj.allocate(n);
      }

      template <typename U, typename... Args >
      constexpr void construct(U* p, Args &&... args)
      {
         obj.construct(p, std::forward<Args>(args)...);
      }

      template <typename U, typename Size>
      void deallocate(U* p, Size n)
      {
         obj.deallocate(p, n);
      }
   };
}