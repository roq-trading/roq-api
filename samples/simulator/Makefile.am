bin_PROGRAMS = simulator
simulator_LDFLAGS = \
	$(GFLAGS_LIBS) \
	$(LIBGLOG_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(LIBEVENT_LIBS) \
	-levent_pthreads \
	$(top_builddir)/samples/strategy/libstrategy.la
simulator_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/samples \
	$(GFLAGS_CFLAGS) \
	$(LIBGLOG_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(LIBEVENT_CFLAGS)
simulator_SOURCES = main.cpp
