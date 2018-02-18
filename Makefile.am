ACLOCAL_AMFLAGS = -I m4

SUBDIRS = . include src tests examples

dist_doc_DATA = LICENSE README.md ChangeLog.md

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc

check-local:
	cd ${GTEST_DIR} && $(MAKE) $(AM_MAKEFLAGS) lib/libgtest.la lib/libgtest_main.la

.PHONY: lint

lint:
	cpplint --linelength=120 --recursive include/ src/ tests/ examples/

cppcheck:
	cppcheck --language=c++ --std=c++11 include/ examples/
