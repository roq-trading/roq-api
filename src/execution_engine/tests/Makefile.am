TESTS = execution-engine-tests
check_PROGRAMS = execution-engine-tests
execution_engine_tests_LDADD = \
	$(GTEST_LIBS)
execution_engine_tests_CPPFLAGS = \
	-I$(top_builddir)/src \
	$(GTEST_CFLAGS)
nodist_execution_engine_tests_SOURCES = \
	main.cpp
