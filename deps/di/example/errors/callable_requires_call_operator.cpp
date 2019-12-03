//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
  struct test_config : di::config {
    struct dummy {};
    static auto policies(...) { return di::make_policies(dummy{}); }
  };

  // clang-format off
  di::make_injector<test_config>();
  /** policy<test_config::dummy>::requires_<call_operator> **/
  // clang-format on
}
