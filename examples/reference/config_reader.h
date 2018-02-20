/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <string>
#include <ucl++.h>
#include "reference/config.h"

namespace examples {
namespace reference {

class ConfigReader final {
 public:
  explicit ConfigReader(const std::string& filename);
  Config parse() const;

 private:
  ConfigReader() = delete;
  ConfigReader(ConfigReader&) = delete;
  ConfigReader& operator=(ConfigReader&) = delete;

 private:
  ucl::Ucl _config;
};

}  // namespace reference
}  // namespace examples

