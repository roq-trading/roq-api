/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/config_reader.h"

#include <glog/logging.h>

#include <string>
#include <utility>

namespace examples {
namespace reference {

namespace {
  static std::string parse_string(const libconfig::Setting& setting) {
    return setting.c_str();
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
  static std::pair<double, double> parse_model_params(const libconfig::Setting& setting) {
    return std::make_pair(parse_double(setting.lookup("fast")),
                          parse_double(setting.lookup("slow")));
  }
}  // namespace

ConfigReader::ConfigReader(const std::string& config_file) {
  _config.readFile(config_file.c_str());
}

Config ConfigReader::parse() const {
  const auto& root = _config.getRoot();
  return Config {
    .time_zone    = parse_time_zone(root.lookup("time_zone")),
    .instrument   = parse_string(root.lookup("instrument")),
    .exchange     = parse_string(root.lookup("exchange")),
    .model_params = parse_model_params(root.lookup("model_params")),
  };
}

}  // namespace reference
}  // namespace examples
