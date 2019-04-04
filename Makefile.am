SUBDIRS = . include src test

if ENABLE_DOC
SUBDIRS += doc
endif

dist_doc_DATA = LICENSE README.md

ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc

.PHONY: lint

lint:
	cpplint --linelength=120 \
		--recursive include/ \
		--recursive src/ \
		--recursive test/
