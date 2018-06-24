TESTS = roq-api-test
check_PROGRAMS = roq-api-test
roq_api_test_LDADD = \
	$(top_builddir)/src/api/libroq.la \
	$(GTEST_LIBS) \
	$(SPDLOG_LIBS) \
	$(LIBUNWIND_LIBS)
roq_api_test_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(GTEST_CFLAGS) \
	$(SPDLOG_CFLAGS) \
	$(LIBUNWIND_CFLAGS)
nodist_roq_api_test_SOURCES = \
	test_codec.cpp \
	test_connection_string.cpp \
	test_gateways.cpp \
	test_logging.cpp \
	test_stream.cpp \
	main.cpp
