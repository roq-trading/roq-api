/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/compat/fmt.hpp"

#include <fmt/format.h>

#include <type_traits>
#include <utility>

#include "roq/message_info.hpp"
#include "roq/name.hpp"

namespace roq {

//! Event
template <typename T>
struct Event final {
  using value_type = std::remove_cvref_t<T>;

  Event(MessageInfo const &message_info, T const &value) : message_info{message_info}, value{value} {}

  Event(Event &&) = delete;
  Event(Event const &) = delete;

  void operator=(Event &&) = delete;
  void operator=(Event const &) = delete;

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

  //! Create event and dispatch to handler
  template <typename... Args>
  static void create_and_dispatch(auto &handler, MessageInfo const &message_info, T const &value, Args &&...args) {
    Event const event{message_info, value};
    return event.template dispatch<void>(handler, std::forward<Args>(args)...);
  }
};

//! Create event and dispatch to handler
template <typename T, typename... Args>
inline void create_event_and_dispatch(auto &handler, MessageInfo const &message_info, T const &value, Args &&...args) {
  return Event<T>::create_and_dispatch(handler, message_info, value, std::forward<Args>(args)...);
}

}  // namespace roq

template <typename T>
struct fmt::formatter<roq::Event<T>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<T> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"({}={}, )"
        R"(message_info={})"
        R"(}})"sv,
        roq::get_name<T>(),
        event.value,
        event.message_info);
  }
};
