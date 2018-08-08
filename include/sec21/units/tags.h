#pragma once

namespace sec21::units
{
   namespace detail
   {
      // non-templated base classes which enable RTTI testing.
      struct base_unit_tag {};
      struct dimension_tag {};
      struct unit_tag {};
   }
}
