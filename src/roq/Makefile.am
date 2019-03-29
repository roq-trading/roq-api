SUBDIRS = core stream unwind logging
lib_LTLIBRARIES = libroq-api.la
libroq_api_la_LDFLAGS = \
	-avoid-version
libroq_api_la_LIBADD = \
	$(top_builddir)/src/roq/logging/libroq-api-logging.la \
	$(top_builddir)/src/roq/unwind/libroq-api-unwind.la \
	$(top_builddir)/src/roq/stream/libroq-api-stream.la \
	$(top_builddir)/src/roq/core/libroq-api-core.la \
	$(SPDLOG_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(GFLAGS_LIBS) \
	$(FMTLIB_LIBS)
libroq_api_la_CPPFLAGS = \
	-I$(top_builddir)/include \
	-I$(top_srcdir)/src
libroq_api_la_SOURCES = \
	application.cpp
