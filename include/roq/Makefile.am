pkgincludedir=$(includedir)/roq
pkginclude_HEADERS = \
	enums_generated.h api_generated.h \
	api.h stream.h codec.h client.h simulation.h \
	platform.h filesystem.h file.h memory.h net.h utils.h \
	libevent.h \
	logging.h

EXTRA_DIST=api.fbs enums.fbs

BUILT_SOURCES=enums_generated.h api_generated.h

CLEANFILES=enums_generated.h api_generated.h

api_generated.h: ${srcdir}/api.fbs
	$(FLATC) --cpp --python --scoped-enums $<

enums_generated.h: ${srcdir}/enums.fbs
	$(FLATC) --cpp --python --scoped-enums $<
