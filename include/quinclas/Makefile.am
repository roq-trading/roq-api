pkgincludedir=$(includedir)/quinclas
pkginclude_HEADERS=tradingapi.h net.h libevent.h codec.h client.h \
	enums_generated.h logging.h tradingapi_generated.h

EXTRA_DIST=tradingapi.fbs enums.fbs

BUILT_SOURCES=enums_generated.h tradingapi_generated.h

CLEANFILES=enums_generated.h tradingapi_generated.h

tradingapi_generated.h: ${srcdir}/tradingapi.fbs
	$(FLATC) --cpp --python --scoped-enums $<

enums_generated.h: ${srcdir}/enums.fbs
	$(FLATC) --cpp --python --scoped-enums $<
