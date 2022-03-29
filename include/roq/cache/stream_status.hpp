/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/mask.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct StreamStatus final {
  StreamStatus() {}

  StreamStatus(uint16_t stream_id, const std::string_view &account) : stream_id(stream_id), account(account) {}

  StreamStatus(const StreamStatus &) = delete;
  StreamStatus(StreamStatus &&) = default;

  void clear() { status = {}; }

  [[nodiscard]] bool operator()(const roq::StreamStatus &stream_status) {
    auto dirty = false;
    dirty |= utils::update(supports, Mask<SupportType>{stream_status.supports});
    dirty |= utils::update(status, stream_status.status);
    dirty |= utils::update(type, stream_status.type);
    dirty |= utils::update(priority, stream_status.priority);
    return dirty;
  }

  [[nodiscard]] operator roq::StreamStatus() const {
    return {
        .stream_id = stream_id,
        .account = account,
        .supports = supports.get(),
        .status = status,
        .type = type,
        .priority = priority,
    };
  }

  const uint16_t stream_id = {};
  const Account account;

  Mask<SupportType> supports = {};
  ConnectionStatus status = {};
  StreamType type = {};
  Priority priority = {};
};

}  // namespace cache
}  // namespace roq
