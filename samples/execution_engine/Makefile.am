SUBDIRS = . tests
bin_PROGRAMS = execution-engine
execution_engine_LDFLAGS = \
	$(GFLAGS_LIBS) \
	$(LIBGLOG_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(LIBEVENT_LIBS) \
	-levent_pthreads
execution_engine_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/samples \
	$(GFLAGS_CFLAGS) \
	$(LIBGLOG_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(LIBEVENT_CFLAGS)
execution_engine_SOURCES = \
	strategy.cpp \
	main.cpp
EXTRA_DIST = \
	strategy.h
