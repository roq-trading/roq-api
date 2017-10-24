AC_INIT([tradingapi], [0.1], [thraneh@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
# are we on debian?
AC_CHECK_TOOL([DPKG_DEB], [dpkg-deb])
AC_CHECK_TOOL([DPKG_ARCHITECTURE], [dpkg-architecture])
AC_CHECK_TOOL([FAKEROOT], [fakeroot])
AM_CONDITIONAL([HAVE_DEB], [test x$DPKG_DEB != x -a x$DPKG_ARCHITECTURE != x -a x$FAKEROOT != x])
AS_IF([test x$HAVE_DEB != xno], [
  DEB_BUILD_ARCH=`${DPKG_ARCHITECTURE} -qDEB_BUILD_ARCH 2>/dev/null`
  AC_SUBST([DEB_BUILD_ARCH])
  DEB_BUILD_MULTIARCH=`${DPKG_ARCHITECTURE} -qDEB_BUILD_MULTIARCH 2>/dev/null`
  AC_SUBST([DEB_BUILD_MULTIARCH])
  DEB_PKG_NAME=${PACKAGE_NAME}
  AC_SUBST([DEB_PKG_NAME])
  DEB_PKG_VERSION=${PACKAGE_VERSION}
  AC_SUBST([DEB_PKG_VERSION])
  DEB_PKG_REVISION=1
  AC_SUBST([DEB_PKG_REVISION])
  DEB_PKG_FILE=${PACKAGE_NAME}_${DEB_PKG_VERSION}-${DEB_PKG_REVISION}_${DEB_BUILD_ARCH}
  AC_SUBST([DEB_PKG_FILE])
  DEB_PKG_EMAIL=${PACKAGE_BUGREPORT}
  AC_SUBST([DEB_PKG_EMAIL])
  AC_CONFIG_FILES([
    packaging/debian/control
    packaging/debian/postinst
    packaging/debian/prerm
  ])
])
# are we on rpm?
AC_CHECK_TOOL([RPM], [rpm])
AC_CHECK_TOOL([RPMBUILD], [rpmbuild])
AM_CONDITIONAL([HAVE_RPM], [test x$RPM != x -a x$RPMBUILD != x])
AS_IF([test x$HAVE_RPM != xno], [
  AC_MSG_RESULT([yes])
  RPM_ARCH=`${RPM} --eval '%{_arch}' 2>/dev/null`
  AC_SUBST(RPM_ARCH)
  RPM_PKG_FILE=${PACKAGE_NAME}-${PACKAGE_VERSION}-1.${RPM_ARCH}.rpm
  AC_SUBST(RPM_PKG_FILE)
  AC_CONFIG_FILES([
    packaging/rpm/SPECS/tradingapi.spec
  ])
], [
  AC_MSG_RESULT([no])
])
AC_CONFIG_FILES([
 Makefile
 include/Makefile
 include/quinclas/Makefile
])
AC_OUTPUT
