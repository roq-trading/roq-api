TESTS = roq-api-test
check_PROGRAMS = roq-api-test

# SO 35998856
check_LTLIBRARIES = libgtest.la
libgtest_la_SOURCES = \
	$(top_srcdir)/third_party/googletest/googletest/src/gtest-all.cc
libgtest_la_CPPFLAGS = \
	-I$(top_srcdir)/third_party/googletest/googletest/include \
	-I$(top_srcdir)/third_party/googletest/googletest
libgtest_la_LDFLAGS = -pthread

roq_api_test_LDADD = \
	$(top_builddir)/src/api/libroq.la \
	libgtest.la \
	$(SPDLOG_LIBS) \
	$(LIBUNWIND_LIBS)
roq_api_test_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/third_party/googletest/googletest/include \
	$(SPDLOG_CFLAGS) \
	$(LIBUNWIND_CFLAGS)
nodist_roq_api_test_SOURCES = \
	test_codec.cpp \
	test_connection_string.cpp \
	test_gateways.cpp \
	test_logging.cpp \
	test_stream.cpp \
	test_utils.cpp \
	main.cpp
