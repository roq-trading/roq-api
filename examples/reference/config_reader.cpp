/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/config_reader.h"

#include <quinclas/logging.h>

#include <fstream>
#include <string>
#include <utility>

namespace examples {
namespace reference {

namespace {

static cctz::time_zone parse_time_zone(const std::string& value) {
  cctz::time_zone result;
  if (!cctz::load_time_zone(value, &result))
    throw std::runtime_error("Unable to initialize time-zone object");
  return result;
}

static std::chrono::seconds parse_duration(double value) {
  return std::chrono::seconds(static_cast<int>(value));
}

static std::pair<double, double> parse_model_params(const ucl::Ucl& setting) {
  return std::make_pair(setting["fast"].number_value(),
                        setting["slow"].number_value());
}

static ucl::Ucl read_file(const std::string& filename) {
  std::ifstream file(filename);
  std::string data((std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>());
  std::string error;
  auto result = ucl::Ucl::parse(data, error);
  if (!error.empty())
    throw std::runtime_error(error);
  return result;
}

}  // namespace

ConfigReader::ConfigReader(const std::string& filename)
    : _config(read_file(filename)) {}

Config ConfigReader::parse() const {
  return Config {
    .time_zone     = parse_time_zone(_config["time_zone"].string_value()),
    .risk_limit    = _config["risk_limit"].number_value(),
    .order_timeout = parse_duration(_config["order_timeout"].number_value()),
    .exchange      = _config["exchange"].string_value(),
    .instrument    = _config["instrument"].string_value(),
    .model_params  = parse_model_params(_config["model_params"]),
  };
}

}  // namespace reference
}  // namespace examples
