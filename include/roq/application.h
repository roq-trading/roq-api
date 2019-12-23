/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {

class ROQ_PUBLIC Application {
 public:
  Application(
      int argc,
      char **argv,
      const std::string_view& description,
      const std::string_view& version = ROQ_VERSION);

  Application(Application&&) = default;

  virtual ~Application();

  int run();

 protected:
  virtual int main(int argc, char **argv) = 0;

 private:
  Application(const Application&) = delete;
  void operator=(const Application&) = delete;

 private:
  char **_argv;
  int _argc;
};

}  // namespace roq
