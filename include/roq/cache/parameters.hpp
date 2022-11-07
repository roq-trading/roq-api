/* Copyright (c) 2017-2022, Hans Erik Thrane */

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
    for (auto &[key, value] : parameters_)
      buffer.emplace_back(key, value);  // note! copy
    return {
        .parameters = buffer,
        .update_type = UpdateType::SNAPSHOT,
        .user = context.user,
    };
  }

 protected:
  bool update_incremental(auto const &parameters_update) {
    auto result = false;
    for (auto &[key, value] : parameters_update.parameters)
      result |= utils::update(parameters_[key], value);
    return result;
  }

  void update_snapshot(auto const &parameters_update) {
    parameters_.clear();
    for (auto &[key, value] : parameters_update.parameters)
      parameters_.emplace(key, value);
  }

 private:
  absl::node_hash_map<ParameterKey, ParameterValue> parameters_;
};

}  // namespace cache
}  // namespace roq
