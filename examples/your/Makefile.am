lib_LTLIBRARIES = libexample-strategy.la
libexample_strategy_la_LDFLAGS = -static
libexample_strategy_la_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	$(FLATBUFFERS_CFLAGS)
libexample_strategy_la_SOURCES = strategy.cpp
EXTRA_libexample_strategy_la_SOURCES = strategy.h
