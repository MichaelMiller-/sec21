#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <memory>

#include <sec21/resource.h>

// possible resource types
namespace mock
{
   namespace legacy
   {
      template <typename T>
      auto free_store_new(T* ptr)
      {
         return ptr;
      }
      template <typename T>
      auto free_store_delete(T* ptr)
      {
         delete ptr;
      }
   }
   namespace opengl
   {
      using GLsizei = std::size_t;
      using GLuint = int;

      void glGenBuffers(GLsizei n, GLuint* p)
      {
         std::ignore = n;
         static GLuint next{ 1 };
         *p = next++;
      }
      void glDeleteBuffers(GLsizei n, GLuint const* p)
      {
         std::ignore = n;
         if (*p == 0)
         {
            // do nothing
         }
         else
         {
            // free buffer memory
         }
      }
   }
   namespace file
   {
      int open_file()
      {
         static int next{ 1 };
         return next++;
      }
      void close_file(int id)
      {
         if (id == -1)
         {
            // do nothing
         }
         else
         {
            // close file
         }
      }
   }
}

// -> pattern
// a function to acquire
// a function to release

namespace behavior
{
   template <typename T>
   struct free_store
   {
      using handle_t = T*;

      handle_t null_handle() const noexcept
      {
         return nullptr;
      }
      handle_t acquire(T* ptr)
      {
         return mock::legacy::free_store_new<T>(ptr);
      }
      void release(handle_t const& h)
      {
         mock::legacy::free_store_delete(h);
      }
   };

   struct vbo
   {
      struct vbo_handle
      {
         mock::opengl::GLuint    id;
         mock::opengl::GLsizei   n;
      };

      using handle_t = vbo_handle;

      handle_t null_handle() const noexcept
      {
         return { 0, 0 };
      }
      handle_t acquire(std::size_t n)
      {
         handle_t result;
         mock::opengl::glGenBuffers(1, &result.id);
         result.n = n;
         return result;
      }
      void release(handle_t const& h)
      {
         mock::opengl::glDeleteBuffers(h.n, &h.id);
      }
   };

   struct file
   {
      using handle_t = int;

      handle_t null_handle() const noexcept
      {
         return -1;
      }
      handle_t acquire()
      {
         return mock::file::open_file();
      }
      void release(handle_t const& h)
      {
         mock::file::close_file(h);
      }
   };
}

//! \todo write some more tests
TEST_CASE("generic unique resource class", "[core]")
{
   using namespace sec21;

   resource<behavior::file> res1;
   REQUIRE(res1.operator bool() == false);

   resource<behavior::file> res2{ mock::file::open_file() };
   REQUIRE(res2.operator bool() == true);
}
