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
	$(top_builddir)/src/roq/libroq-api.la \
	$(ROQ_LOGGING_LIBS) \
	$(FMT_LIBS) \
	$(GFLAGS_LIBS) \
	libgtest.la
roq_api_test_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	$(PTHREAD_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(FMT_CFLAGS) \
	$(ROQ_LOGGING_CFLAGS) \
	$(GTEST_CFLAGS)
nodist_roq_api_test_SOURCES = \
	alignment.cpp \
	format.cpp \
	metrics.cpp \
	span.cpp \
	string.cpp \
	traits.cpp \
	utils.cpp \
	main.cpp
