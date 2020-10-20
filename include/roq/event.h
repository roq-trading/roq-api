/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/message_info.h"

namespace roq {

//! Event
template <typename T>
struct Event final {
  Event(const MessageInfo &message_info, const T &value)
      : message_info(message_info), value(value) {}

  Event(const Event &) = delete;

  //! Dispatch to handler
  template <typename H>
  void dispatch(H &&handler) {
    handler(*this);
  }

  //! Access MessageInfo
  operator const MessageInfo &() const { return message_info; }

  //! Access Message
  operator const T &() const { return value; }

  const MessageInfo &message_info;  //!< MessageInfo
  const T &value;                   //!< Message
};

//! Create event and dispatch to handler
template <typename H, typename T>
inline void create_event_and_dispatch(
    H &&handler, const MessageInfo &message_info, const T &value) {
  Event event(message_info, value);
  event.dispatch(handler);
}

}  // namespace roq
