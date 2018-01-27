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
    /*
    for (const auto& iter : result)
      LOG(INFO) << iter.first << " " << iter.second;
    */
    return result;
  }
}  // namespace

Config::Config(const std::string& config_file)
    : Config(std::move(ConfigReader(config_file))) {
  LOG(INFO) << "gateways = {";
  for (const auto& iter : gateways)
    LOG(INFO) << "    " << iter.first << ": " << iter.second;
  LOG(INFO) << "}";
}

Config::Config(ConfigReader&& config_reader)
    : gateways(std::move(parse_gateways(config_reader.get().getRoot().lookup("gateways")))) {}

Config::ConfigReader::ConfigReader(const std::string& config_file) {
  _config.readFile(config_file.c_str());
}

}  // namespace reference
}  // namespace examples
