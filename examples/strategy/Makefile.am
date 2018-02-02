bin_PROGRAMS = example-strategy
example_strategy_LDFLAGS = \
	$(top_builddir)/src/tradingapi/libquinclas-tradingapi.la \
	$(CCTZ_LIBS) \
	$(DOUBLE_CONVERSION_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBGLOG_LIBS)
example_strategy_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	$(CCTZ_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBGLOG_CFLAGS)
example_strategy_SOURCES = strategy.cpp main.cpp
EXTRA_example_strategy_SOURCES = strategy.h
