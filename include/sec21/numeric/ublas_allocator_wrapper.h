#pragma once

#include <memory>

namespace sec21::numeric
{
   //! \todo boost::numeric::ublas has problem with std::allocator deprecation 
   template <typename T>
   struct ublas_allocator_wrapper
   {
      T obj{};

      using value_type =  typename std::allocator_traits<T>::value_type;
      using difference_type = typename std::allocator_traits<T>::difference_type;
      using size_type = typename std::allocator_traits<T>::size_type;
      using iterator = typename std::allocator_traits<T>::pointer;
      using const_iterator = typename std::allocator_traits<T>::const_pointer;

      template <typename Size>
      [[nodiscard]] constexpr auto allocate(Size n)
      {
         return obj.allocate(n);
      }

      template <typename U, typename... Args >
      constexpr void construct(U* p, Args &&... args)
      {
         std::allocator_traits<T>::construct(obj, p, std::forward<Args>(args)...);
      }

      template <typename U, typename Size>
      void deallocate(U* p, Size n)
      {
         obj.deallocate(p, n);
      }
   };
}