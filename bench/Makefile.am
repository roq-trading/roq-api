TESTS = roq-api-bench
check_PROGRAMS = roq-api-bench
roq_api_bench_LDADD = \
	$(top_builddir)/src/api/libroq.la \
	$(CCTZ_LIBS) \
	$(DOUBLE_CONVERSION_LIBS) \
	$(LIBUNWIND_LIBS) \
	$(BENCHMARK_LIBS)  \
	-lpthread
roq_api_bench_CFLAGS = -pthread
roq_api_bench_CPPFLAGS = \
	-I$(top_builddir)/src \
	$(CCTZ_CFLAGS) \
	$(DOUBLE_CONVERSION_CFLAGS) \
	$(LIBUNWIND_CFLAGS) \
	$(BENCHMARK_CFLAGS)
nodist_roq_api_bench_SOURCES = \
	main.cpp
