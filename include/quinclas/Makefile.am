SUBDIRS=. utils tests

pkgincludedir=$(includedir)/quinclas
pkginclude_HEADERS=tradingapi.h tradingapi_generated.h common_generated.h codec.h

EXTRA_DIST = \
	tradingapi.fbs \
	common.fbs

BUILT_SOURCES = \
	tradingapi_generated.h \
	common_generated.h

CLEANFILES = \
	tradingapi_generated.h \
	common_generated.h

tradingapi_generated.h: ${srcdir}/tradingapi.fbs
	$(FLATC) --cpp --scoped-enums $<

common_generated.h: ${srcdir}/common.fbs
	$(FLATC) --cpp --scoped-enums $<
