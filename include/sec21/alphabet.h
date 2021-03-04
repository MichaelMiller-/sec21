#pragma once

#include <sec21/strong_type.h>
#include <sec21/policy/compare.h>
#include <sec21/policy/print.h>
#include <sec21/policy/increment_alphabet.h>

#include <nlohmann/json.hpp>

#include <string>

namespace sec21
{
   using alphabet_t = strong_type<
      std::string, 
      struct alphabet_tag, 
      policy::increment_alphabet, 
      policy::compare,
      policy::print>;

   void to_json(nlohmann::json &j, alphabet_t const &obj)
   {
      j = nlohmann::json{ { "value", static_cast<alphabet_t::underlying_t const&>(obj) } };
   }
}
