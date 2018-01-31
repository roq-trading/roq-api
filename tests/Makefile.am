TESTS = tradingapi-tests
check_PROGRAMS = tradingapi-tests
tradingapi_tests_LDADD = \
	$(top_builddir)/src/tradingapi/libquinclas-tradingapi.la \
	$(GTEST_LIBS) \
	$(CCTZ_LIBS)
tradingapi_tests_CPPFLAGS = $(GTEST_CFLAGS) $(CCTZ_CFLAGS) -I$(top_srcdir)/include
nodist_tradingapi_tests_SOURCES = test_codec.cpp main.cpp
