//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <cassert>
#include <iostream>

namespace di = boost::di;

struct iworld {
  virtual ~iworld() noexcept = default;
};
struct world : iworld {
  world() { std::cout << " world!" << std::endl; }
};

struct hello {
  explicit hello(int i) {
    assert(42 == i);
    std::cout << "hello";
  }
};

/// aggregate initialization `example{hello, world}`
struct example {
  hello h;
  iworld& w;
};

int main() {
  // clang-format off
  auto injector = di::make_injector(
    di::bind<iworld>().to<world>()
  , di::bind<>().to(42)
  );
  // clang-format off

  injector.create<example>();
}
