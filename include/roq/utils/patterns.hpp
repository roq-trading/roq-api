/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

namespace roq {

// overloaded
// source: https://en.cppreference.com/w/cpp/utility/variant/visit

template <typename... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// partial
// https://stackoverflow.com/a/39470219

template <typename Function, typename... Arguments>
auto partial(Function func, Arguments... args) {
  return [=](auto... rest) { return func(args..., rest...); };
}

}  // namespace roq
