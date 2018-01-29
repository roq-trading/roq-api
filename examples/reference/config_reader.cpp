/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/config_reader.h"

#include <glog/logging.h>

#include <string>
#include <utility>

namespace examples {
namespace reference {

namespace {
  static libconfig::Setting& lookup(const libconfig::Setting& setting, const char *name) {
    return setting[name];
    // not available on Ubuntu 14.04:
    // return setting.lookup(name);
  }
  static std::string parse_string(const libconfig::Setting& setting) {
    return setting.c_str();
  }
  static int parse_int(const libconfig::Setting& setting) {
    return static_cast<int>(setting);
  }
  static double parse_double(const libconfig::Setting& setting) {
    switch (setting.getType()) {
      case libconfig::Setting::TypeInt:
        return static_cast<int>(setting);
      case libconfig::Setting::TypeInt64:
        return static_cast<long long>(setting);  // NOLINT
      case libconfig::Setting::TypeFloat:
        return static_cast<double>(setting);
      default:
        throw std::runtime_error("Unable to parse double");
    }
  }
  static cctz::time_zone parse_time_zone(const libconfig::Setting& setting) {
    const std::string time_zone(parse_string(setting));
    cctz::time_zone result;
    if (!cctz::load_time_zone(time_zone, &result))
      throw std::runtime_error("Unable to initialize time-zone object");
    return result;
  }
  static std::chrono::seconds parse_seconds(const libconfig::Setting& setting) {
    return std::chrono::seconds(parse_int(setting));
  }
  static std::pair<double, double> parse_model_params(const libconfig::Setting& setting) {
    return std::make_pair(parse_double(lookup(setting, "fast")),
                          parse_double(lookup(setting, "slow")));
  }
}  // namespace

ConfigReader::ConfigReader(const std::string& config_file) {
  _config.readFile(config_file.c_str());
}

Config ConfigReader::parse() const {
  const auto& root = _config.getRoot();
  return Config {
    .time_zone     = parse_time_zone(lookup(root, "time_zone")),
    .instrument    = parse_string(lookup(root, "instrument")),
    .exchange      = parse_string(lookup(root, "exchange")),
    .order_timeout = parse_seconds(lookup(root, "order_timeout_seconds")),
    .model_params  = parse_model_params(lookup(root, "model_params")),
  };
}

}  // namespace reference
}  // namespace examples
