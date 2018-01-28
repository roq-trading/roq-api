bin_PROGRAMS = example-reference
example_reference_LDFLAGS = \
	$(CCTZ_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(LIBCONFIG_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBGLOG_LIBS)
example_reference_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	$(CCTZ_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBCONFIG_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBGLOG_CFLAGS)
example_reference_SOURCES = position_manager.cpp order_manager.cpp strategy.cpp config.cpp config_reader.cpp main.cpp
EXTRA_example_reference_SOURCES = position_manager.h order_manager.cpp strategy.h config.h config_reader.h
