/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/message_info.hpp"

namespace roq {

//! Event
template <typename T>
struct Event final {
  using value_type = T const;

  Event(const MessageInfo &message_info, const value_type &value) : message_info(message_info), value(value) {}

  Event(const Event &) = delete;

  //! Dispatch to handler
  template <typename Result, typename Handler, typename... Args>
  Result dispatch(Handler &&handler, Args &&...args) const {
    return handler(*this, std::forward<Args>(args)...);
  }

  //! Access MessageInfo
  operator const MessageInfo &() const { return message_info; }

  //! Access Message
  operator const value_type &() const { return value; }

  //! Structured binding
  operator std::pair<const MessageInfo &, const value_type &>() const { return {message_info, value}; }

  const MessageInfo &message_info;  //!< MessageInfo
  const value_type &value;          //!< Message
};

//! Create event and dispatch to handler
template <typename Handler, typename T, typename... Args>
inline void create_event_and_dispatch(
    Handler &&handler, const MessageInfo &message_info, const T &value, Args &&...args) {
  const Event event{message_info, value};
  return event.template dispatch<void>(handler, std::forward<Args>(args)...);
}

}  // namespace roq
