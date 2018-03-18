bin_PROGRAMS = example-strategy
example_strategy_LDFLAGS = \
	$(top_builddir)/src/tradingapi/libquinclas-tradingapi.la \
	$(CCTZ_LIBS) \
	$(CROSSGUID_LIBS) \
	$(DOUBLE_CONVERSION_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(SPDLOG_LIBS)
example_strategy_CPPFLAGS = \
	-DQUINCLAS_SPDLOG \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	$(CCTZ_CFLAGS) \
	$(CROSSGUID_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(SPDLOG_CFLAGS)
example_strategy_SOURCES = strategy.cpp main.cpp
EXTRA_example_strategy_SOURCES = strategy.h
