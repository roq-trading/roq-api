TESTS = tradingapi-tests
check_PROGRAMS = tradingapi-tests
tradingapi_tests_LDADD = \
	$(GTEST_LIBS)
tradingapi_tests_CPPFLAGS = \
	-I$(top_builddir)/src \
	$(GTEST_CFLAGS)
nodist_tradingapi_tests_SOURCES = \
	main.cpp
