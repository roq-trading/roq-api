SUBDIRS = .
bin_PROGRAMS = execution-engine
execution_engine_LDFLAGS = \
	$(GFLAGS_LIBS) \
	$(LIBGLOG_LIBS) \
	$(LIBCONFIG_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(LIBEVENT_LIBS) \
	-levent_pthreads
execution_engine_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	$(GFLAGS_CFLAGS) \
	$(LIBGLOG_CFLAGS) \
	$(LIBCONFIG_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(LIBEVENT_CFLAGS)
execution_engine_SOURCES = \
	net.cpp \
	event.cpp \
	dispatcher.cpp \
	controller.cpp \
	strategy.cpp \
	main.cpp
EXTRA_DIST = \
	net.h \
	event.h \
	dispatcher.h \
	controller.h \
	strategy.h
