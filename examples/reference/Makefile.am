bin_PROGRAMS = example-reference
example_reference_LDFLAGS = \
	$(GFLAGS_LIBS) \
	$(LIBGLOG_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBCONFIG_LIBS) \
	$(CCTZ_LIBS)
example_reference_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	$(GFLAGS_CFLAGS) \
	$(LIBGLOG_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBCONFIG_CFLAGS) \
	$(CCTZ_CFLAGS)
example_reference_SOURCES = config.cpp position.cpp strategy.cpp main.cpp
EXTRA_example_reference_SOURCES = config.h position.h strategy.h
