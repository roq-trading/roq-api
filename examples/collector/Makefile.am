bin_PROGRAMS = example-collector
example_collector_LDFLAGS = \
	$(GFLAGS_LIBS) \
	$(LIBGLOG_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(CCTZ_LIBS)
example_collector_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	$(GFLAGS_CFLAGS) \
	$(LIBGLOG_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(CCTZ_CFLAGS)
example_collector_SOURCES = collector.cpp main.cpp
EXTRA_example_collector_SOURCES = collector.h
