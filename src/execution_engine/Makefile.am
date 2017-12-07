SUBDIRS = . tests
bin_PROGRAMS = execution-engine
execution_engine_LDFLAGS = \
	$(GFLAGS_LIBS) \
	$(LIBGLOG_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(LIBEVENT_LIBS) \
	-levent_pthreads \
	-L/Users/thraneh/tmp/cctz-master \
	-lcctz
execution_engine_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	$(GFLAGS_CFLAGS) \
	$(LIBGLOG_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	-I/Users/thraneh/tmp/cctz-master/include
execution_engine_SOURCES = \
	strategy.cpp \
	main.cpp
EXTRA_DIST = \
	strategy.h
