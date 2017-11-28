SUBDIRS = . include src

dist_doc_DATA = LICENSE README.md VERSION

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc

check-local:
	cd ${GTEST_DIR} && $(MAKE) $(AM_MAKEFLAGS) lib/libgtest.la lib/libgtest_main.la

.PHONY: lint

lint:
	cpplint --linelength=120 --recursive include/quinclas
