/* Copyright (c) 2017-2026, Hans Erik Thrane */

#define CATCH_CONFIG_RUNNER

#include <catch2/catch_session.hpp>

int main(int argc, char **argv) {
  return Catch::Session{}.run(argc, argv);
}
