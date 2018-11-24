bin_PROGRAMS = roq-api-benchmark
roq_api_benchmark_LDADD = \
	$(top_builddir)/src/api/libroq.la \
	$(LIBUNWIND_LIBS) \
	$(BENCHMARK_LIBS)  \
	$(GFLAGS_LIBS)  \
	-lpthread
roq_api_benchmark_CFLAGS = -pthread
roq_api_benchmark_CPPFLAGS = \
	-I$(top_srcdir)/src \
	-I$(top_builddir)/include \
	$(LIBUNWIND_CFLAGS) \
	$(BENCHMARK_CFLAGS) \
	$(GFLAGS_CFLAGS)
nodist_roq_api_benchmark_SOURCES = \
	main.cpp
