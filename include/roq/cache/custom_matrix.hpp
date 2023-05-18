/* Copyright (c) 2017-2023, Hans Erik Thrane */

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

struct CustomMatrix final {
  explicit CustomMatrix(CustomMatrixUpdate const &custom_matrix_update)
      : label{custom_matrix_update.label}, version{custom_matrix_update.version},
        sending_time_utc{custom_matrix_update.sending_time_utc}, user{custom_matrix_update.user} {}
  explicit CustomMatrix(roq::CustomMatrix const &custom_matrix)
      : label{custom_matrix.label}, version{custom_matrix.version} {}

  CustomMatrix(CustomMatrix const &) = delete;
  CustomMatrix(CustomMatrix &&) = default;

  [[nodiscard]] bool operator()(CustomMatrixUpdate const &custom_matrix_update) {
    switch (custom_matrix_update.update_type) {
      using enum UpdateType;
      case UNDEFINED:
      case INCREMENTAL:
      case SNAPSHOT:
        update(custom_matrix_update);
        return true;
      case STALE:
        assert(false);  // not supported
        break;
    }
    return false;
  }

  [[nodiscard]] bool operator()(roq::CustomMatrix const &custom_matrix) {
    switch (custom_matrix.update_type) {
      using enum UpdateType;
      case UNDEFINED:
      case INCREMENTAL:
      case SNAPSHOT:
        update(custom_matrix);
        return true;
      case STALE:
        assert(false);  // not supported
        break;
    }
    return false;
  }

  [[nodiscard]] bool operator()(Event<CustomMatrixUpdate> const &event) { return (*this)(event.value); }

  template <typename Context>
  [[nodiscard]] CustomMatrixUpdate convert(Context const &context) const {
    return {
        .label = label,
        .account = context.account,
        .exchange = context.exchange,
        .symbol = context.symbol,
        .rows = rows,
        .columns = columns,
        .data = data,
        .update_type = UpdateType::SNAPSHOT,
        .version = version,
        .sending_time_utc = sending_time_utc,
        .user = user,
    };
  }

  uint16_t stream_id = {};

  Label const label;
  std::vector<MatrixKey> rows;
  std::vector<MatrixKey> columns;
  std::vector<double> data;
  uint32_t version = {};
  std::chrono::nanoseconds sending_time_utc = {};
  User const user;

 protected:
  template <typename T>
  void update(T const &custom_matrix) {
    auto make_copy = [&](auto &target, auto const &source) {
      target.resize(std::size(source));
      std::copy(std::begin(source), std::end(source), std::begin(target));
    };
    make_copy(rows, custom_matrix.rows);
    make_copy(columns, custom_matrix.columns);
    make_copy(data, custom_matrix.data);
    utils::update(version, custom_matrix.version);
    if constexpr (std::is_same<T, CustomMatrixUpdate>::value) {
      utils::update(sending_time_utc, custom_matrix.sending_time_utc);
    }
  }
};

}  // namespace cache
}  // namespace roq
