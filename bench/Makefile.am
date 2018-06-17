TESTS = roq-api-bench
check_PROGRAMS = roq-api-bench
roq_api_bench_LDADD = \
	$(top_builddir)/src/api/libroq.la \
	$(LIBUNWIND_LIBS) \
	$(BENCHMARK_LIBS)  \
	-lpthread
roq_api_bench_CFLAGS = -pthread
roq_api_bench_CPPFLAGS = \
	-I$(top_srcdir)/src \
	-I$(top_builddir)/include \
	$(LIBUNWIND_CFLAGS) \
	$(BENCHMARK_CFLAGS)
nodist_roq_api_bench_SOURCES = \
	main.cpp
