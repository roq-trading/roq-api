/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <tuple>
#include <utility>

#include "roq/message_info.h"

namespace roq {

//! Event
template <typename T>
struct Event final {
  Event(const MessageInfo &message_info, const T &value)
      : message_info(message_info), value(value) {}

  Event(const Event &) = delete;

  //! Dispatch to handler
  template <typename R, typename H, typename... Args>
  R dispatch(H &&handler, Args &&...args) {
    return handler(*this, std::forward<Args>(args)...);
  }

  //! Access MessageInfo
  operator const MessageInfo &() const { return message_info; }

  //! Access Message
  operator const T &() const { return value; }

  //! Structured binding
  operator std::tuple<const MessageInfo &, const T &>() const { return {message_info, value}; }

  const MessageInfo &message_info;  //!< MessageInfo
  const T &value;                   //!< Message
};

//! Create event and dispatch to handler
template <typename H, typename T, typename... Args>
inline void create_event_and_dispatch(
    H &&handler, const MessageInfo &message_info, const T &value, Args &&...args) {
  Event event(message_info, value);
  return event.template dispatch<void>(handler, std::forward<Args>(args)...);
}

}  // namespace roq
