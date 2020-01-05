lib_LTLIBRARIES = libroq-api.la
libroq_api_la_LDFLAGS = \
	-avoid-version
libroq_api_la_LIBADD = \
	$(SPDLOG_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(GFLAGS_LIBS) \
	$(FMT_LIBS)
libroq_api_la_CPPFLAGS = \
	-I$(top_builddir)/include \
	-I$(top_srcdir)/src \
	$(FMT_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(SPDLOG_CFLAGS)
libroq_api_la_SOURCES = \
	affinity.cpp \
	application.cpp \
	builtins.cpp \
	logging.cpp \
	metrics.cpp \
	unwind.cpp
