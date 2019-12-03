//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_WRAPPER_HPP
#define BOOST_DI_FAKE_WRAPPER_HPP

struct fake_wrapper {
  template <class T>
  inline operator T() const {
    return {};
  }
};

#endif
