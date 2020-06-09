/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

namespace roq {

template <typename E>
struct event_value_helper;

template <typename E>
const typename event_value_helper<E>::type& event_value(const E& e) {
  using helper = event_value_helper<E>;
  return static_cast<const typename helper::type&>(helper(e));
}

template <typename E, typename MessageInfo>
E create_event(const MessageInfo&, const E&);

}  // namespace roq
