#pragma once

#include <type_traits>
#include <set>
#include <iterator>

namespace sec21::structural_analysis
{
   //! \todo constexpr lambda
   struct less_id
   {
      template <typename T>
      constexpr bool operator () (T&& lhs, T&& rhs) const noexcept
      {
         return lhs.id < rhs.id;
      }
   };

   //! \todo benchmark and/or test expensive function
   template <typename Iterator>
   [[nodiscard]] bool has_duplicate_ids(Iterator first, Iterator last) noexcept
   {
      using value_t = std::decay_t<std::iterator_traits<Iterator>::value_type>;
      const auto result = std::set<value_t, less_id>(first, last);
      //! \todo c++20 use std::ssize -> remove using and cast
      using diff_t = std::common_type_t<std::ptrdiff_t, std::make_signed_t<decltype(result.size())>>;
      return static_cast<diff_t>(std::size(result)) < std::distance(first, last);
   }

   template <typename System>
   [[nodiscard]] bool has_duplicate_node_ids(System const& sys) noexcept
   {
      return has_duplicate_ids(std::begin(sys.nodes), std::end(sys.nodes));
   }

   //! \todo benchmark and/or test expensive function
   template <typename System>
   [[nodiscard]] bool is_valid(System const& sys) noexcept
   {
      // return all_of()
      return
         // logic problems
         has_duplicate_node_ids(sys) == false &&
         // statische probleme
         std::size(sys.nodes) > 2 && std::size(sys.members) > 1 && std::size(sys.adjacency_list) > 1;
   }
}
