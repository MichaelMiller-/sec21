//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

int main() {
  struct dummy {};
  // clang-format off
  di::make_injector(
    di::bind<int>().in(dummy{})); /** scope<dummy>::requires_<create> **/
  );
  // clang-format on
}
