TESTS = api-benchmark
check_PROGRAMS = api-benchmark
api_benchmark_LDADD = \
	$(top_builddir)/src/api/libroq.la \
	$(BENCHMARK_LIBS)  \
	-lpthread
api_benchmark_CFLAGS = -pthread
api_benchmark_CPPFLAGS = \
	-I$(top_builddir)/src \
	$(BENCHMARK_CFLAGS)
nodist_api_benchmark_SOURCES = \
	main.cpp
