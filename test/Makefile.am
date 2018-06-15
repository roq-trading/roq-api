TESTS = roq-api-test
check_PROGRAMS = roq-api-test
roq_api_test_LDADD = \
	$(top_builddir)/src/api/libroq.la \
	$(CCTZ_LIBS) \
	$(DOUBLE_CONVERSION_LIBS) \
	$(GLOG_LIBS) \
	$(GTEST_LIBS) \
	$(SPDLOG_LIBS) \
	$(LIBUNWIND_LIBS)
roq_api_test_CPPFLAGS = \
	-I$(top_srcdir)/include \
	$(GTEST_CFLAGS) \
	$(CCTZ_CFLAGS) \
	$(DOUBLE_CONVERSION_CFLAGS) \
	$(GLOG_CFLAGS) \
	$(GTEST_CFLAGS) \
	$(SPDLOG_CFLAGS) \
	$(LIBUNWIND_CFLAGS)
nodist_roq_api_test_SOURCES = \
	test_codec.cpp \
	test_connection_string.cpp \
	test_gateways.cpp \
	test_logging.cpp \
	test_logging_glog.cpp \
	test_logging_spdlog.cpp \
	test_stream.cpp \
	main.cpp
