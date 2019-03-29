/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {

class ROQ_PUBLIC Application : NonCopyable {
 public:
  Application(
      int argc,
      char **argv,
      const char *description,
      const char *version = ROQ_VERSION);
  virtual ~Application();

  int run();

 protected:
  virtual int main(int argc, char **argv) = 0;

 private:
  char **_argv;
  int _argc;
};

}  // namespace roq
