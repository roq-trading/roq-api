AC_REVISION([m4_esyscmd_s([build-aux/git-version])])

AC_INIT([roq-api],
        m4_esyscmd_s([build-aux/git-version]),
        [thraneh@gmail.com],
        [roq-api])

AC_PREREQ([2.59])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AC_CONFIG_FILES([
 Makefile
 schema/Makefile
 schema/cpp/Makefile
 schema/fbs/Makefile
 include/Makefile
 include/roq/Makefile
 include/roq/client/Makefile
 include/roq/metrics/Makefile
 test/Makefile
])
AC_CONFIG_FILES([include/roq/api.h])
AC_CONFIG_FILES([roq-api.pc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AM_PROG_AR
LT_INIT([disable-dynamic])
AC_PROG_CXX
AC_LANG([C++])

AX_PTHREAD

ROQ_CHECK_STDCXX

ROQ_CHECK_FMT
ROQ_CHECK_FLATBUFFERS

ROQ_CHECK_GTEST([third_party/googletest])

AC_ARG_ENABLE([doxygen],
  [AS_HELP_STRING([--enable-doxygen], [Enable documentation])],,
  [enable_doxygen=no])

AS_IF([test "x$enable_doxygen" = "xyes"], [
  AC_CHECK_PROG(HAVE_DOXYGEN, [doxygen], [yes], [no])
  ],[
  HAVE_DOXYGEN=no
  ])

AM_CONDITIONAL([ENABLE_DOXYGEN],
  [test "x$enable_doxygen" = "xyes"])

AS_IF([test "x$enable_doxygen" = "xyes"],
      [AC_CONFIG_FILES([doxygen/Makefile doxygen/Doxyfile])])

AC_SUBST([ROQ_VERSION_MAJOR], m4_esyscmd_s([build-aux/git-version | sed -e 's/^\([0-9]*\)\.[0-9]*\.[0-9]*.*/\1/']))
AC_SUBST([ROQ_VERSION_MINOR], m4_esyscmd_s([build-aux/git-version | sed -e 's/^[0-9]*\.\([0-9]*\)\.[0-9]*.*/\1/']))
AC_SUBST([ROQ_VERSION_PATCH], m4_esyscmd_s([build-aux/git-version | sed -e 's/^[0-9]*\.[0-9]*\.\([0-9]*\).*/\1/']))

AC_OUTPUT
