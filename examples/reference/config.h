/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <string>
#include <unordered_map>
#include <libconfig.h++>

namespace examples {
namespace reference {

struct Config {
 public:
  explicit Config(const std::string& config_file);

 private:
  // simple wrapper of libconfig::Config (mainly to support c++11 move semantics)
  class ConfigReader final {
   public:
    explicit ConfigReader(const std::string& config_file);
    const libconfig::Setting& get(const char *lookup) const;

   private:
    ConfigReader() = delete;
    ConfigReader(ConfigReader&) = delete;
    ConfigReader& operator=(ConfigReader&) = delete;

   private:
    libconfig::Config _config;
  };
  explicit Config(ConfigReader&& config_reader);

 private:
  Config() = delete;
  Config(Config&) = delete;
  Config& operator=(Config&) = delete;

 public:
  const std::unordered_map<std::string, std::string> gateways;
};

}  // namespace reference
}  // namespace examples

