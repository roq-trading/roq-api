bin_PROGRAMS = example-collector
example_collector_LDFLAGS = \
	$(top_builddir)/src/tradingapi/libquinclas-tradingapi.la \
	$(CCTZ_LIBS) \
	$(DOUBLE_CONVERSION_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(GLOG_LIBS) \
	$(LIBEVENT_LIBS)
example_collector_CPPFLAGS = \
	-DQUINCLAS_GLOG \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	$(CCTZ_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(GLOG_CFLAGS) \
	$(LIBEVENT_CFLAGS)
example_collector_SOURCES = collector.cpp main.cpp
EXTRA_example_collector_SOURCES = collector.h
