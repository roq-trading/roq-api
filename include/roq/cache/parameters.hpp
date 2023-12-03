/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <absl/container/node_hash_map.h>

#include <array>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct Parameters final {
  Parameters() = default;

  Parameters(Parameters const &) = delete;
  Parameters(Parameters &&) = default;

  [[nodiscard]] bool operator()(ParametersUpdate const &parameters_update) {
    switch (parameters_update.update_type) {
      using enum UpdateType;
      case UNDEFINED:
      case INCREMENTAL:
        return update_incremental(parameters_update);
      case SNAPSHOT:
        update_snapshot(parameters_update);
        return true;
      case STALE:
        assert(false);  // not supported
        break;
    }
    return false;
  }

  [[nodiscard]] ParametersUpdate convert(auto &context, std::vector<Parameter> &buffer) const {
    buffer.clear();
    for (auto &[label, tmp] : parameters_)
      for (auto &[key_2, value] : tmp) {
        auto &[account, exchange, symbol] = key_2;
        Parameter parameter{
            .label = label,        // note! copy
            .account = account,    // note! copy
            .exchange = exchange,  // note! copy
            .symbol = symbol,      // note! copy
            .value = value,        // note! copy
        };
        buffer.emplace_back(std::move(parameter));
      }
    return {
        .parameters = buffer,
        .update_type = UpdateType::SNAPSHOT,
        .user = context.user,
    };
  }

  template <typename Callback>
  bool get_all(Callback callback) const {
    auto result = false;
    for (auto &[label, tmp] : parameters_)
      for (auto &[key_2, value] : tmp) {
        result = true;
        auto &[account, exchange, symbol] = key_2;
        callback(label, account, exchange, symbol, value);
      }
    return result;
  }

  template <typename Callback>
  bool get_labels(Callback callback) const {
    auto result = false;
    for (auto &[label, _] : parameters_) {
      result = true;
      callback(label);
    }
    return result;
  }

  template <typename Callback>
  bool get(std::string_view const &label, Callback callback) const {
    auto result = false;
    auto iter = parameters_.find(label);
    if (iter != std::end(parameters_))
      for (auto &[key_2, value] : (*iter).second) {
        result = true;
        auto &[account, exchange, symbol] = key_2;
        callback(account, exchange, symbol, value);
      }
    return result;
  }

 protected:
  bool update_incremental(auto const &parameters_update) {
    auto result = false;
    for (auto &[label, account, exchange, symbol, value] : parameters_update.parameters) {
      SecondaryKey key_2{account, exchange, symbol};
      result |= utils::update(parameters_[label][key_2], value);
    }
    return result;
  }

  void update_snapshot(auto const &parameters_update) {
    parameters_.clear();
    for (auto &[label, account, exchange, symbol, value] : parameters_update.parameters) {
      SecondaryKey key_2{account, exchange, symbol};
      parameters_[label][key_2] = value;
    }
  }

 private:
  using SecondaryKey = std::tuple<std::string, std::string, std::string>;
  absl::node_hash_map<std::string, absl::node_hash_map<SecondaryKey, std::string>> parameters_;
};

}  // namespace cache
}  // namespace roq
