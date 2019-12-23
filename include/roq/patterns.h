/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

namespace roq {

// overloaded
// source: https://en.cppreference.com/w/cpp/utility/variant/visit

template<class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

}  // namespace roq
