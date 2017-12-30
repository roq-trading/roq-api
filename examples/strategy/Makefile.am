bin_PROGRAMS = example-strategy
example_strategy_LDFLAGS = \
	$(GFLAGS_LIBS) \
	$(LIBGLOG_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(CCTZ_LIBS)
example_strategy_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	$(GFLAGS_CFLAGS) \
	$(LIBGLOG_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(CCTZ_CFLAGS)
example_strategy_SOURCES = strategy.cpp main.cpp
EXTRA_example_strategy_SOURCES = strategy.h
