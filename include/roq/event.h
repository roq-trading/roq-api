/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

namespace roq {

namespace detail {
template <typename E>
struct event_value_helper;
}  // namespace detail

// utility function to get the value from an event without
// having to specify the name of the member
// useful for creating higher level templated utility functions

template <typename E>
const typename detail::event_value_helper<E>::type& event_value(const E& e) {
  using helper = detail::event_value_helper<E>;
  return static_cast<const typename helper::type&>(helper(e));
}

// utility function to create an event from a message info
// structure and an actual event
// useful for creating higher level templated utility functions

template <typename E, typename MessageInfo>
E create_event(const MessageInfo&, const E&);

}  // namespace roq
