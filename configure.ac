m4_include(m4/ax_cxx_compile_stdcxx.m4)  # https://www.gnu.org/software/autoconf-archive/ax_cxx_compile_stdcxx.html

AC_INIT([Quinclas TradingAPI],
        [0.1],
        [thraneh@gmail.com],
        [quinclas-tradingapi])

AC_PREREQ([2.59])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_FILES([
 Makefile
 include/Makefile
 include/quinclas/Makefile
 tests/Makefile
 examples/Makefile
 examples/strategy/Makefile
 examples/collector/Makefile
 examples/reference/Makefile
 examples/gateway/Makefile
])
AC_CONFIG_FILES([include/quinclas/tradingapi.h])
AC_CONFIG_FILES([quinclas-tradingapi.pc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Check required programs.
AM_PROG_AR
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_LIBTOOL

# Check C++ 11 support.
AX_CXX_COMPILE_STDCXX(11, noext, mandatory)

# Find glog.
PKG_CHECK_MODULES([LIBGLOG], [libglog], [], [
  AC_CHECK_HEADER(
    [glog/logging.h],
    [AC_SUBST([LIBGLOG_CFLAGS])],
    [AC_MSG_ERROR([unable to find glog header files])]
  )
  AC_SUBST([LIBGLOG_LIBS], [-lglog])
])

# Find gflags.
PKG_CHECK_MODULES([GFLAGS], [gflags], [], [
  AC_CHECK_HEADER(
    [gflags/gflags.h],
    [AC_SUBST([GFLAGS_CFLAGS])],
    [AC_MSG_ERROR([unable to find gflags header files])]
  )
  AC_SUBST([GFLAGS_LIBS], [-lgflags])
])

# Find double-conversion.
PKG_CHECK_MODULES([DOUBLE_CONVERSION], [cctz], [], [
  AC_CHECK_HEADER(
    [double-conversion/double-conversion.h],
    [AC_SUBST([DOUBLE_CONVERSION_CFLAGS])],
    [AC_MSG_ERROR([unable to find double-conversion header files])]
  )
  AC_SUBST([DOUBLE_CONVERSION_LIBS], [-ldouble-conversion])
])

# Find cctz.
PKG_CHECK_MODULES([CCTZ], [cctz], [], [
  AC_CHECK_HEADER(
    [cctz/civil_time.h],
    [AC_SUBST([CCTZ_CFLAGS])],
    [AC_MSG_ERROR([unable to find cctz header files])]
  )
  AC_SUBST([CCTZ_LIBS], [-lcctz])
])

# Find flatbuffers.
PKG_CHECK_MODULES([FLATBUFFERS], [flatbuffers], [], [
  AC_CHECK_HEADER(
    [flatbuffers/flatbuffers.h],
    [AC_SUBST([FLATBUFFERS_CFLAGS])],
    [AC_MSG_ERROR([unable to find flatbuffers header files])]
  )
  AC_SUBST([FLATBUFFERS_LIBS], [-lflatbuffers])
])
AC_PATH_PROGS([FLATC], [flatc], [])

# Find libevent.
PKG_CHECK_MODULES([LIBEVENT], [libevent], [], [
  AC_CHECK_HEADER(
    [event2/event.h],
    [AC_SUBST([LIBEVENT_CFLAGS])],
    [AC_MSG_ERROR([unable to find libevent header files])]
  )
  AC_SUBST([LIBEVENT_LIBS], [-levent])
])

# Find libconfig.
PKG_CHECK_MODULES([LIBCONFIG], [libconfig++])

# Initialize googletest.
AC_CONFIG_SUBDIRS([googletest/googletest])
AC_SUBST([GTEST_DIR], ['${top_builddir}/googletest/googletest'])
AC_SUBST([GTEST_CFLAGS], ['-I${GTEST_DIR}/include'])
AC_SUBST([GTEST_LIBS], ['${GTEST_DIR}/lib/libgtest.la ${GTEST_DIR}/lib/libgtest_main.la'])

AC_OUTPUT
