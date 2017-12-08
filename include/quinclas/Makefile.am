SUBDIRS=. io client server

pkgincludedir=$(includedir)/quinclas
pkginclude_HEADERS=tradingapi.h enums_generated.h tradingapi_generated.h codec.h

EXTRA_DIST=tradingapi.fbs enums.fbs

BUILT_SOURCES=enums_generated.h tradingapi_generated.h

CLEANFILES=enums_generated.h tradingapi_generated.h

tradingapi_generated.h: ${srcdir}/tradingapi.fbs
	$(FLATC) --cpp --scoped-enums $<

enums_generated.h: ${srcdir}/enums.fbs
	$(FLATC) --cpp --scoped-enums $<
