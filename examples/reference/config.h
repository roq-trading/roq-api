/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <string>
#include <unordered_map>

namespace examples {
namespace reference {

class Config final {
 public:
  explicit Config(const std::string& config_file);

 private:
  Config() = delete;
  Config(Config&) = delete;
  Config& operator=(Config&) = delete;

 public:
  const std::unordered_map<std::string, std::string> gateways;
};

}  // namespace reference
}  // namespace examples

