pkgincludedir=$(includedir)/quinclas/codec
pkginclude_HEADERS=enums_generated.h tradingapi_generated.h codec.h

EXTRA_DIST=tradingapi.fbs enums.fbs

BUILT_SOURCES=enums_generated.h tradingapi_generated.h

CLEANFILES=enums_generated.h tradingapi_generated.h

tradingapi_generated.h: ${srcdir}/tradingapi.fbs
	$(FLATC) --cpp --python --scoped-enums $<

enums_generated.h: ${srcdir}/enums.fbs
	$(FLATC) --cpp --python --scoped-enums $<
