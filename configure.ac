m4_include(m4/ax_cxx_compile_stdcxx.m4)  # https://www.gnu.org/software/autoconf-archive/ax_cxx_compile_stdcxx.html

AC_INIT([Roq],
        [0.1.2],
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
])
AC_CONFIG_FILES([include/roq/api.h])
AC_CONFIG_FILES([roq.pc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Check required programs.
AM_PROG_AR
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_LIBTOOL

# Check C++ 11 support.
AX_CXX_COMPILE_STDCXX(11, noext, mandatory)

# Check required dependencies.
ROQ_CHECK_LIBUNWIND
ROQ_CHECK_CROSSGUID
ROQ_CHECK_SPDLOG
ROQ_CHECK_GLOG
ROQ_CHECK_GFLAGS
ROQ_CHECK_DOUBLE_CONVERSION
ROQ_CHECK_CCTZ
ROQ_CHECK_FLATBUFFERS
ROQ_CHECK_LIBEVENT
ROQ_CHECK_LIBUCL

# Initialize googletest.
AC_CONFIG_SUBDIRS([third_party/googletest/googletest])
AC_SUBST([GTEST_DIR], ['${top_builddir}/third_party/googletest/googletest'])
AC_SUBST([GTEST_CFLAGS], ['-I${GTEST_DIR}/include'])
AC_SUBST([GTEST_LIBS], ['${GTEST_DIR}/lib/libgtest.la ${GTEST_DIR}/lib/libgtest_main.la'])

AC_OUTPUT
