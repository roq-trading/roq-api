AC_REVISION([m4_esyscmd_s([build-aux/git-version])])

AC_INIT([roq],
        m4_esyscmd_s([build-aux/git-version]),
        [thraneh@gmail.com],
        [roq])

AC_PREREQ([2.59])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AC_CONFIG_FILES([
 Makefile
 include/Makefile
 include/roq/Makefile
 src/Makefile
 src/api/Makefile
 test/Makefile
 doc/Makefile
 doc/Doxyfile
])
AC_CONFIG_FILES([include/roq/api.h])
AC_CONFIG_FILES([roq.pc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AM_PROG_AR
LT_INIT
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_LIBTOOL

ROQ_CHECK_STDCXX

ROQ_CHECK_LIBUNWIND
ROQ_CHECK_CROSSGUID
ROQ_CHECK_SPDLOG
ROQ_CHECK_GFLAGS
ROQ_CHECK_FLATBUFFERS
ROQ_CHECK_LIBEVENT
ROQ_CHECK_LIBUCL

AC_ARG_ENABLE([benchmark],
	      AS_HELP_STRING([--enable-benchmark], [Enable benchmark]),
	      [],
	      [enable_benchmark=no])

AS_IF([test "x$enable_benchmark" = "xyes"],
      [AC_CONFIG_FILES([src/benchmark/Makefile])])

AS_IF(
  [test "x$enable_benchmark" = "xyes"],
  [ROQ_CHECK_BENCHMARK])

AM_CONDITIONAL([ENABLE_BENCHMARK],
               [test "x$enable_benchmark" = "xyes"])

AC_ARG_ENABLE([doc],
  [AS_HELP_STRING([--enable-doc], [Enable documentation])],,
  [enable_doc=no])

AS_IF([test "x$enable_doc" = "xyes"], [
  AC_CHECK_PROG(HAVE_DOXYGEN, [doxygen], [yes], [no])
  AC_CHECK_PROG(HAVE_DOT, [dot], [yes], [no])
  ],[
  HAVE_DOXYGEN=no
  HAVE_DOT=no
  ])

AM_CONDITIONAL([ENABLE_DOC],
	       [test "x$enable_doc" = "xyes"])

AC_SUBST([ROQ_VERSION_MAJOR], m4_esyscmd_s([build-aux/git-version | sed -e 's/^\([0-9]*\)\.[0-9]*\.[0-9]*.*/\1/']))
AC_SUBST([ROQ_VERSION_MINOR], m4_esyscmd_s([build-aux/git-version | sed -e 's/^[0-9]*\.\([0-9]*\)\.[0-9]*.*/\1/']))
AC_SUBST([ROQ_VERSION_PATCH], m4_esyscmd_s([build-aux/git-version | sed -e 's/^[0-9]*\.[0-9]*\.\([0-9]*\).*/\1/']))

AC_OUTPUT
