/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <string>
#include <libconfig.h++>
#include "reference/config.h"

namespace examples {
namespace reference {

class ConfigReader final {
 public:
  explicit ConfigReader(const std::string& config_file);
  Config parse() const;

 private:
  ConfigReader() = delete;
  ConfigReader(ConfigReader&) = delete;
  ConfigReader& operator=(ConfigReader&) = delete;

 private:
  libconfig::Config _config;
};

}  // namespace reference
}  // namespace examples

