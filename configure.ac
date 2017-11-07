AC_INIT([tradingapi], m4_esyscmd_s([cat include/quinclas/tradingapi.h | grep '#define QUINCLAS_VERSION' | cut -d'"' -f2]), [thraneh@gmail.com])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
# debian, ubuntu
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
    packaging/deb/control
    packaging/deb/postinst
    packaging/deb/prerm
  ])
])
# rhel, centos, fedora
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
# darwin
AC_CHECK_TOOL([PKGBUILD], [pkgbuild])
AC_CHECK_TOOL([PRODUCTBUILD], [productbuild])
AM_CONDITIONAL([HAVE_PKG], [test x$PKGBUILD != x -a x$PRODUCTBUILD != x])
AS_IF([test x$HAVE_PKG != xno], [
  AC_MSG_RESULT([yes])
  PKG_ARCH=`uname -p 2>/dev/null`
  AC_SUBST(PKG_ARCH)
  PKG_COMP=osx.pkg
  PKG_FILE=${PACKAGE_NAME}-${PACKAGE_VERSION}-${target_cpu}.pkg
  PKG_IDENT=com.tickcapture
  AC_SUBST(PKG_COMP)
  AC_SUBST(PKG_FILE)
  AC_SUBST(PKG_IDENT)
  AC_CONFIG_FILES([
    packaging/pkg/conclusion.html
    packaging/pkg/distribution.xml
    packaging/pkg/license.html
    packaging/pkg/welcome.html
  ])
], [
  AC_MSG_RESULT([no])
])

AC_CONFIG_FILES([
 Makefile
 include/Makefile
 include/quinclas/Makefile
 quinclas-tradingapi.pc
])
AC_OUTPUT
