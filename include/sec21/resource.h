#pragma once

#include <algorithm>

namespace sec21
{
   //! \todo Concept ResourceBehavior

   // inspired by the std::unique_ptr interface
   template <typename Behavior>
   class resource : Behavior
   {
    public:
      using behavior_t = Behavior;
      using handle_t = typename behavior_t::handle_t;
      //! \todo Behavior::hash_function

    private:
      handle_t handle;

      auto& behavior() noexcept { return static_cast<behavior_t&>(*this); }
      const auto& behavior() const noexcept { return static_cast<const behavior_t&>(*this); }

    public:
      resource() noexcept : handle{behavior().null_handle()} {}

      ~resource() noexcept { reset(); }

      explicit resource(handle_t const& h) noexcept : handle{h} {}

      resource(resource const&) = delete;
      auto& operator=(resource const&) = delete;

      resource(resource&& rhs) noexcept : handle{rhs.handle} { rhs.release(); }
      auto& operator=(resource&& rhs) noexcept
      {
         reset();
         handle = rhs.handle;
         rhs.reset();
         return *this;
      }

      // return the current handle without changing it
      [[nodiscard]] auto get() const noexcept { return handle; }

      // return the current handle and then set the current handle to null handle
      [[nodiscard]] auto release() noexcept
      {
         auto tmp{handle};
         handle = behavior().null_handle();
         return tmp;
      }

      void reset() noexcept
      {
         behavior().release(handle);
         handle = behavior().null_handle();
      }
      void reset(handle_t const& h) noexcept
      {
         behavior().release(handle);
         handle = h;
      }

      void swap(resource& rhs) noexcept
      {
         // enable ADL lookup; see https://en.cppreference.com/w/cpp/algorithm/swap
         using std::swap;
         swap(handle, rhs.handle);
      }

      explicit operator bool() const noexcept { return handle != behavior().null_handle(); }

      friend bool operator==(resource const& lhs, resource const& rhs) noexcept { return lhs.handle == rhs.handle; }

      friend bool operator!=(resource const& lhs, resource const& rhs) noexcept { return !(lhs == rhs); }

      friend void swap(resource& lhs, resource& rhs) noexcept { lhs.swap(rhs); }
   };
} // namespace sec21
