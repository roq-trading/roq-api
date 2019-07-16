# SO 35998856
check_LTLIBRARIES = libgtest.la
libgtest_la_SOURCES = \
	$(GTEST_DIR)/googletest/src/gtest-all.cc
libgtest_la_CPPFLAGS = \
	$(GTEST_CFLAGS)
libgtest_la_LDFLAGS = \
	$(PTHREAD_CFLAGS)

TESTS = roq-api-test
check_PROGRAMS = roq-api-test
roq_api_test_LDADD = \
	$(PTHREAD_CFLAGS) \
	$(top_builddir)/src/roq/logging/libroq-api-logging.la \
	$(top_builddir)/src/roq/unwind/libroq-api-unwind.la \
	$(top_builddir)/src/roq/stream/libroq-api-stream.la \
	$(top_builddir)/src/roq/core/libroq-api-core.la \
	$(SPDLOG_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(GFLAGS_LIBS) \
	$(FMT_LIBS) \
	libgtest.la
roq_api_test_CPPFLAGS = \
	$(PTHREAD_CFLAGS) \
	$(GTEST_CFLAGS) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src
nodist_roq_api_test_SOURCES = \
	alignment.cpp \
	format.cpp \
	logging.cpp \
	metrics.cpp \
	static.cpp \
	stream.cpp \
	traits.cpp \
	main.cpp
