bin_PROGRAMS = reference-strategy
reference_strategy_LDFLAGS = \
	$(CCTZ_LIBS) \
	$(FLATBUFFERS_LIBS) \
	$(GFLAGS_LIBS) \
	$(LIBCONFIG_LIBS) \
	$(LIBEVENT_LIBS) \
	$(LIBGLOG_LIBS)
reference_strategy_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/examples \
	$(CCTZ_CFLAGS) \
	$(FLATBUFFERS_CFLAGS) \
	$(GFLAGS_CFLAGS) \
	$(LIBCONFIG_CFLAGS) \
	$(LIBEVENT_CFLAGS) \
	$(LIBGLOG_CFLAGS)
reference_strategy_SOURCES = \
  config.cpp \
  config_reader.cpp \
  gateway_manager.cpp \
  order.cpp \
  order_manager.cpp \
  position_manager.cpp \
  risk_manager.cpp \
  trading_model.cpp \
  utilities.cpp \
  main.cpp
EXTRA_reference_strategy_SOURCES = \
  config.h \
  config_reader.h \
  gateway_manager.h \
  order.h \
  order_manager.h \
  position_manager.h \
  risk_manager.h \
  trading_model.h \
  utilities.h
configdir=$(datadir)/quinclas/examples
config_DATA = strategy.conf
