/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/config.h"
#include <glog/logging.h>

namespace examples {
namespace reference {

namespace {
  static std::unordered_map<std::string, std::string> parse_gateways(const libconfig::Setting& setting) {
    std::unordered_map<std::string, std::string> result;
    for (auto& iter : setting) {
      const char *name = iter.getName();
      const char *value = iter;
      result.emplace(std::make_pair(name, value));
    }
    return result;
  }
}  // namespace

Config::Config(const std::string& config_file) : Config(std::move(ConfigReader(config_file))) {}

Config::Config(ConfigReader&& config_reader)
    : gateways(std::move(parse_gateways(config_reader.get("gateways")))) {
  // log the configuration
  LOG(INFO) << "gateways = {";
  for (const auto& iter : gateways)
    LOG(INFO) << "    " << iter.first << ": " << iter.second;
  LOG(INFO) << "}";
}

Config::ConfigReader::ConfigReader(const std::string& config_file) {
  _config.readFile(config_file.c_str());
}

const libconfig::Setting& Config::ConfigReader::get(const char *name) const {
  return _config.getRoot().lookup(name);
}

}  // namespace reference
}  // namespace examples
