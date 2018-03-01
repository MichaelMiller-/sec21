#pragma once

namespace sec21 {
namespace policies
{
   struct no_policy
   {
      template <typename T>
      static void apply(T& t) {}
   };

   template <size_t N>
   //! \todo rename
   struct clear_after_N_entries
   {
      template <typename T>
      static void apply(T& t) 
      {
         if (t.size() > N)
            t.clear();
      }
   };
}
}