m4_include(m4/ax_cxx_compile_stdcxx.m4)  # https://www.gnu.org/software/autoconf-archive/ax_cxx_compile_stdcxx.html

AC_INIT([quinclas-tradingapi], m4_esyscmd([tr -d '\n' < VERSION]), [thraneh@gmail.com])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AC_DISABLE_STATIC
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AM_PROG_AR
LT_INIT
AC_PROG_CXX

AX_CXX_COMPILE_STDCXX(11, noext, mandatory)

PKG_CHECK_MODULES([LIBGLOG], [libglog])
PKG_CHECK_MODULES([LIBCONFIG], [libconfig++])
PKG_CHECK_MODULES([GFLAGS], [gflags])
PKG_CHECK_MODULES([LIBEVENT], [libevent])
PKG_CHECK_MODULES([FLATBUFFERS], [flatbuffers], [], [
  AC_LANG_PUSH([C++])
  SAVE_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS -I$prefix/include"
  AC_CHECK_HEADER(
    [flatbuffers/flatbuffers.h],
    [AC_SUBST([FLATBUFFERS_CFLAGS])],
    [AC_MSG_ERROR([unable to find flatbuffers header files])]
  )
  CPPFLAGS="$SAVE_CPPFLAGS"
  AC_LANG_POP([C++])
  # if we get to here, simply assume the library exists (it seems very complex to test for the existence)
  AC_SUBST([FLATBUFFERS_LIBS], [-lflatbuffers])
])
SAVE_PATH="$PATH"
PATH="$PATH:$prefix/bin:$prefix/usr/local/bin"
AC_PATH_PROGS([FLATC], [flatc], [])
PATH="$SAVE_PATH"

AC_CONFIG_FILES([
 Makefile
 include/Makefile
 include/quinclas/Makefile
 include/quinclas/tradingapi.h
 include/quinclas/codec/Makefile
 include/quinclas/io/Makefile
 include/quinclas/client/Makefile
 include/quinclas/server/Makefile
 tests/Makefile
 samples/Makefile
 samples/strategy/Makefile
 samples/simulator/Makefile
 samples/execution_engine/Makefile
 quinclas-tradingapi.pc
])

AC_CONFIG_SUBDIRS([
  googletest/googletest
])

AC_SUBST([GTEST_DIR], ['${top_builddir}/googletest/googletest'])
AC_SUBST([GTEST_CFLAGS], ['-I${GTEST_DIR}/include'])
AC_SUBST([GTEST_LIBS], ['${GTEST_DIR}/lib/libgtest.la ${GTEST_DIR}/lib/libgtest_main.la'])

AC_OUTPUT
