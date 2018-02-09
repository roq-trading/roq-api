/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "quinclas/logging.h"

#include <iostream>

namespace quinclas {
namespace logging {
namespace implementation {

#if defined(QUINCLAS_STDLOG)
sink_t null_sink = [](const char * message){};
sink_t info_sink = [](const char *message){ std::cout << "I " << message; };
sink_t warning_sink = [](const char *message){ std::cerr << "W " << message; };
sink_t error_sink = [](const char *message){ std::cerr << "E " << message; };
sink_t fatal_sink = [](const char *message){ std::cerr << "F " << message; std::abort(); };
#endif
}  // namespace implementation

void Logger::initialize(int argc, char *argv[]) {
#if defined(QUINCLAS_GLOG)
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
#elif defined(QUINCLAS_SPDLOG)
#else

#endif
}

}  // namespace logging
}  // namespace quinclas
