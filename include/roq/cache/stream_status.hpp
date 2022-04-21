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

  void clear() { connection_status = {}; }

  [[nodiscard]] bool operator()(const roq::StreamStatus &stream_status) {
    auto dirty = false;
    dirty |= utils::update(supports, stream_status.supports);
    dirty |= utils::update(transport, stream_status.transport);
    dirty |= utils::update(protocol, stream_status.protocol);
    dirty |= utils::update(encoding, stream_status.encoding);
    dirty |= utils::update(priority, stream_status.priority);
    dirty |= utils::update(connection_status, stream_status.connection_status);
    return dirty;
  }

  [[nodiscard]] operator roq::StreamStatus() const {
    return {
        .stream_id = stream_id,
        .account = account,
        .supports = supports,
        .transport = transport,
        .protocol = protocol,
        .encoding = encoding,
        .priority = priority,
        .connection_status = connection_status,
    };
  }

  const uint16_t stream_id = {};
  const Account account;

  Mask<SupportType> supports;
  Transport transport = {};
  Protocol protocol = {};
  Mask<Encoding> encoding = {};
  Priority priority = {};
  ConnectionStatus connection_status = {};
};

}  // namespace cache
}  // namespace roq
