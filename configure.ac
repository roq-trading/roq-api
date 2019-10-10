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
 include/Makefile
 include/roq/Makefile
 src/Makefile
 src/roq/Makefile
 src/roq/core/Makefile
 src/roq/unwind/Makefile
 src/roq/logging/Makefile
 test/Makefile
])
AC_CONFIG_FILES([include/roq/api.h])
AC_CONFIG_FILES([roq-api.pc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AM_PROG_AR
LT_INIT([disable-static])
AC_PROG_CXX
AC_LANG([C++])

AX_PTHREAD

ROQ_CHECK_STDCXX

ROQ_CHECK_FMT
ROQ_CHECK_GFLAGS
ROQ_CHECK_LIBUNWIND
ROQ_CHECK_SPDLOG

ROQ_CHECK_GTEST([third_party/googletest])

AC_ARG_ENABLE([doc],
  [AS_HELP_STRING([--enable-doc], [Enable documentation])],,
  [enable_doc=no])

AS_IF([test "x$enable_doc" = "xyes"], [
  AC_CHECK_PROG(HAVE_DOXYGEN, [doxygen], [yes], [no])
  ],[
  HAVE_DOXYGEN=no
  ])

AM_CONDITIONAL([ENABLE_DOC],
  [test "x$enable_doc" = "xyes"])

AS_IF([test "x$enable_doc" = "xyes"],
      [AC_CONFIG_FILES([doc/Makefile doc/Doxyfile])])

AC_SUBST([ROQ_VERSION_MAJOR], m4_esyscmd_s([build-aux/git-version | sed -e 's/^\([0-9]*\)\.[0-9]*\.[0-9]*.*/\1/']))
AC_SUBST([ROQ_VERSION_MINOR], m4_esyscmd_s([build-aux/git-version | sed -e 's/^[0-9]*\.\([0-9]*\)\.[0-9]*.*/\1/']))
AC_SUBST([ROQ_VERSION_PATCH], m4_esyscmd_s([build-aux/git-version | sed -e 's/^[0-9]*\.[0-9]*\.\([0-9]*\).*/\1/']))

AC_OUTPUT
