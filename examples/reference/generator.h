/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/simulation.h>

#include <chrono>  // NOLINT
#include <string>
#include <utility>

#include "reference/csv_reader.h"

namespace examples {
namespace reference {

class Generator final : public roq::simulation::Generator {
 public:
  explicit Generator(const std::string& path);
  ~Generator();
  std::pair<bool, std::chrono::system_clock::time_point> fetch() override;
  void dispatch(roq::common::Strategy& strategy) override;

 private:
  CsvReader _csv_reader;
  std::chrono::system_clock::time_point _receive_time;
  uint64_t _message_id = 0;
};

}  // namespace reference
}  // namespace examples
