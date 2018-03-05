pkgincludedir=$(includedir)/quinclas
pkginclude_HEADERS=\
	enums_generated.h tradingapi_generated.h \
  tradingapi.h codec.h client.h simulation.h \
  filesystem.h file.h memory.h net.h \
  libevent.h \
  logging.h

EXTRA_DIST=tradingapi.fbs enums.fbs

BUILT_SOURCES=enums_generated.h tradingapi_generated.h

CLEANFILES=enums_generated.h tradingapi_generated.h

tradingapi_generated.h: ${srcdir}/tradingapi.fbs
	$(FLATC) --cpp --python --scoped-enums $<

enums_generated.h: ${srcdir}/enums.fbs
	$(FLATC) --cpp --python --scoped-enums $<
