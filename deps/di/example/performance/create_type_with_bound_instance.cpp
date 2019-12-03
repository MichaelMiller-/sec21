//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cstdlib>
#include <string>
//->
#include <boost/di.hpp>

namespace di = boost::di;

auto test() {
  // clang-format off
  auto injector = di::make_injector(
    di::bind<int>().to(42)
  );
  // clang-format on

  return injector.create<int>();
}

/**
 * ASM x86-64 (same as `return 42`)
 *
 * mov $0x2a,%eax
 * retq
 */

//<-
int main(int, char** argv) {
  std::system(("gdb -batch -ex 'file " + std::string{argv[0]} + "' -ex 'disassemble test'").c_str());
}
//->
