TESTS = tradingapi-tests
check_PROGRAMS = tradingapi-tests
tradingapi_tests_LDADD = \
	$(top_builddir)/src/tradingapi/libquinclas-tradingapi.la \
	$(CCTZ_LIBS) \
	$(DOUBLE_CONVERSION_LIBS) \
	$(GLOG_LIBS) \
	$(GTEST_LIBS) \
	$(SPDLOG_LIBS) \
	$(LIBUNWIND_LIBS)
tradingapi_tests_CPPFLAGS = $(GTEST_CFLAGS) $(CCTZ_CFLAGS) -I$(top_srcdir)/include
nodist_tradingapi_tests_SOURCES = \
	test_connection_string.cpp test_gateways.cpp \
	test_codec.cpp test_logging.cpp test_stream.cpp \
	test_logging_glog.cpp test_logging_spdlog.cpp \
	main.cpp
