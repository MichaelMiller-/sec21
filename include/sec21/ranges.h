#pragma once

#include <range/v3/all.hpp>

namespace sec21 {
   //! \todo intersect is NOT tested
   //! \brief create a intersection of two sets A and B
   //! \link https://en.wikipedia.org/wiki/Intersection_(set_theory)
   //! \param v range of elements to examine 
   template <typename T>
   auto intersect(T && v) {
      return ranges::view::filter([&v](auto && a) {
         return ranges::distance(v | ranges::view::filter([a](auto && b) { return a == b; })) == 1; });
   }

   //! \brief create a symmetric difference of two sets A and B
   //! \link https://en.wikipedia.org/wiki/Symmetric_difference
   //! \param v range of elements to examine 
   template <typename T>
   auto difference(T && v) {
      return ranges::view::remove_if([&v](auto && a) {
         return !ranges::empty(v | ranges::view::filter([&a](auto && b) { return a == b; })); });
   }

}