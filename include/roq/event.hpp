/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/message_info.hpp"

namespace roq {

//! Event
template <typename T, typename = typename std::enable_if<!std::is_const<T>::value>::type>
struct Event final {
  using value_type = T;

  Event(MessageInfo const &message_info_, T const &value_) : message_info{message_info_}, value{value_} {}

  Event(Event const &) = delete;
  Event(Event &&) = delete;

  void operator=(Event const &) = delete;
  void operator=(Event &&) = delete;

  //! Dispatch to handler
  template <typename Result, typename Handler, typename... Args>
  Result dispatch(Handler &&handler, Args &&...args) const {
    return handler(*this, std::forward<Args>(args)...);
  }

  //! Access MessageInfo
  operator MessageInfo const &() const { return message_info; }

  //! Access Message
  operator value_type const &() const { return value; }

  //! Structured binding
  operator std::pair<MessageInfo const &, value_type const &>() const { return {message_info, value}; }

  MessageInfo const &message_info;  //!< MessageInfo
  value_type const &value;          //!< Message
};

//! Create event and dispatch to handler
template <typename Handler, typename T, typename... Args>
inline void create_event_and_dispatch(
    Handler &&handler, MessageInfo const &message_info, T const &value, Args &&...args) {
  Event const event{message_info, value};
  return event.template dispatch<void>(handler, std::forward<Args>(args)...);
}

}  // namespace roq
