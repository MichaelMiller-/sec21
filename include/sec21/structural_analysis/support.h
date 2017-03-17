#pragma  once

namespace sec21 {
namespace structural_analysis {

   //! \todo not a really good design
   enum class support : std::uint32_t
   {
      UNKOWN = 0,
      MOVABLE = 1,   // reaction only in the y-axis
      HINGED,        // reactions in the y & x axis
      // reactions in the y & x axis and in the moments
      //! \todo = 3 // Fixed
      NumberOfTypes
   };

   static constexpr auto reactions(support s) noexcept {
      return static_cast<std::underlying_type<support>::type>(s);
   }

}
}

//constexpr auto degreesOfFreedom(support_t s) {
//  static_assert(true, "implement");
//}
  

//
//template <typename CharType, typename Traits>
//std::basic_ostream<CharType, Traits>& operator << (
//   std::basic_ostream<CharType, Traits> &s,
//   support_t supp)
//{
//   if (supp == support_t::MOVABLE)
//      s << "Movable";
//   if (supp == support_t::HINGED)
//      s << "Hinged";
//   return s;
//}
