#pragma once

#include <system_error>

namespace sec21::structural_analysis
{
   enum class error_code
   {
      invalid_node_name = 1,
      invalid_member_name,
      node_already_exists,
      member_already_exists
   };

   namespace detail
   {
      struct error_code_category : public std::error_category
      {
         virtual const char* name() const noexcept override final { return "StructuralAnalysisError"; }
         
         virtual std::string message(int c) const override final
         {
            switch (static_cast<error_code>(c))
            {
               case error_code::invalid_node_name:
                  return "Invalid node name";
               case error_code::invalid_member_name:
                  return "Invalid node member";
               case error_code::node_already_exists:
                  return "A node with the same name already exists";
               case error_code::member_already_exists:
                  return "A member with the same name already exists";
               default:
                  return "Unknown error";
            }
         }

         virtual std::error_condition default_error_condition(int c) const noexcept override final
         {
            switch (static_cast<error_code>(c))
            {
               case error_code::invalid_node_name:
                  return make_error_condition(std::errc::invalid_argument);
               case error_code::invalid_member_name:
                  return make_error_condition(std::errc::invalid_argument);
               default:
                  return std::error_condition(c, *this);
            }
         }
      };
   }

   extern inline const auto& error_code_category()
   {
      static detail::error_code_category c{};
      return c;
   }

   inline std::error_code make_error_code(error_code e)
   {
      return {static_cast<int>(e), error_code_category()};
   }
}
