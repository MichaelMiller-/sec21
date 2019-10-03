//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <sstream>
#include <vector>

namespace sml = boost::sml;

#if !defined(_MSC_VER)
std::vector<std::string> messages_out;

struct my_logger {
  template <class SM, class TEvent>
  void log_process_event(const TEvent& evt) {
    std::stringstream sstr;
    sstr << evt.c_str();
    messages_out.push_back(sstr.str());
  }

  template <class SM, class TGuard, class TEvent>
  void log_guard(const TGuard&, const TEvent&, bool) {}

  template <class SM, class TAction, class TEvent>
  void log_action(const TAction&, const TEvent&) {}

  template <class SM, class TSrcState, class TDstState>
  void log_state_change(const TSrcState& src, const TDstState& dst) {
    std::stringstream sstr;
    sstr << src.c_str() << " -> " << dst.c_str();
    messages_out.push_back(sstr.str());
  }
};

struct e2 {
  static auto c_str() { return "An Event"; }
};
struct s1_label {
  static auto c_str() { return "A State"; }
};
auto s1 = sml::state<s1_label>;

test logging = [] {
  messages_out.clear();
  // clang-format off
  std::vector<std::string> messages_expected = {
     "e1"
   , "idle -> A State"
   , "An Event"
   , "A State -> terminate"
  };
  // clang-format on

  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
          *"idle"_s + "e1"_e = s1
        , s1 + event<e2> = X
      );
      // clang-format on
    }
  };

  my_logger logger;
  sml::sm<c, sml::logger<my_logger>> sm{logger};
  using namespace sml;
  sm.process_event("e1"_e());
  sm.process_event(e2{});
  expect(messages_out.size() == messages_expected.size());
  expect(std::equal(messages_out.begin(), messages_out.end(), messages_expected.begin()));
};

test logging_entry_exit = [] {
  messages_out.clear();
  // clang-format off
  std::vector<std::string> messages_expected = {
     "on_entry"
   , "e1"
   , "on_exit"
   , "idle -> A State"
   , "on_entry"
   , "An Event"
   , "on_exit"
   , "A State -> terminate"
   , "on_entry"
  };
  // clang-format on

  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
          *"idle"_s + "e1"_e = s1
        , s1 + sml::on_entry<_> / [](){}
        , s1 + sml::on_exit<_> / [](){}
        , s1 + event<e2> = X
      );
      // clang-format on
    }
  };

  my_logger logger;
  sml::sm<c, sml::logger<my_logger>> sm{logger};
  using namespace sml;
  sm.process_event("e1"_e());
  sm.process_event(e2{});
  expect(messages_out.size() == messages_expected.size());
  expect(std::equal(messages_out.begin(), messages_out.end(), messages_expected.begin()));
};
#endif
