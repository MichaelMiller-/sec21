//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <boost/di.hpp>
#include <cassert>

namespace di = boost::di;
//->

auto int1 = [] {};
auto int2 = [] {};

struct T {
  T(int a, int b) : a(a), b(b) {}
  int a = 0;
  int b = 0;
};

namespace boost {
namespace di {
template <>
struct ctor_traits<T> {
  BOOST_DI_INJECT_TRAITS((named = int1) int, (named = int2) int);
};
}
}  // boost::di

int main() {
  // clang-format off
  auto injector = di::make_injector(
    di::bind<int>().named(int1).to(42)
  , di::bind<int>().named(int2).to(87)
  );
  // clang-format on
  auto object = injector.create<T>();
  assert(42 == object.a);
  assert(87 == object.b);
}
