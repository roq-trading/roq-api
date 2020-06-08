lib_LTLIBRARIES = libroq-api.la
libroq_api_la_LDFLAGS = \
	-avoid-version
libroq_api_la_LIBADD = \
	$(FMT_LIBS) \
	$(GFLAGS_LIBS)
libroq_api_la_CPPFLAGS = \
	-I$(top_builddir)/include \
	-I$(top_srcdir)/src \
	$(GFLAGS_CFLAGS) \
	$(FMT_CFLAGS)
libroq_api_la_SOURCES = \
	application.cpp
