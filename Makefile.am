ACLOCAL_AMFLAGS = -I m4

SUBDIRS = . include src test doc

dist_doc_DATA = LICENSE README.md ChangeLog.md

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA = @PACKAGE@.pc

check-local:
	cd ${GTEST_DIR} && $(MAKE) $(AM_MAKEFLAGS) lib/libgtest.la lib/libgtest_main.la

.PHONY: lint

lint:
	cpplint --linelength=120 --recursive include/ src/ test/

cppcheck:
	cppcheck --language=c++ --std=c++11 include/
