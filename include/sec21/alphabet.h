#pragma once

#include <sec21/policy/compare.h>
#include <sec21/policy/increment_alphabet.h>
#include <sec21/policy/print.h>
#include <sec21/strong_type.h>

#include <string>

namespace sec21
{
   // clang-format off
   using alphabet_t = strong_type<
      std::string,
      struct alphabet_tag,
      policy::increment_alphabet,
      policy::compare,
      policy::print>;
   // clang-format on
} // namespace sec21
