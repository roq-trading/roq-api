bin_PROGRAMS = example-gateway-simulator
example_gateway_simulator_LDFLAGS = \
	$(GFLAGS_LIBS) \
	$(LIBGLOG_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(LIBEVENT_LIBS) \
	-levent_pthreads \
	$(top_builddir)/examples/your/libexample-strategy.la
example_gateway_simulator_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	$(GFLAGS_CFLAGS) \
	$(LIBGLOG_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(LIBEVENT_CFLAGS)
example_gateway_simulator_SOURCES = main.cpp
