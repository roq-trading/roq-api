AC_INIT([roq],
        [0.1.6],
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

AM_PROG_AR
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_LIBTOOL

ROQ_CHECK_STDCXX

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

ROQ_CHECK_GTEST([third_party/googletest])

AC_OUTPUT
