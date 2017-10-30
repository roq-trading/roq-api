SUBDIRS=include

.PHONY: lint

lint:
	cpplint --linelength=120 --root=subdir=src/ --recursive include/

if HAVE_DEB
deb: all
	@cd $(abs_builddir) && \
	mkdir -p $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN && \
	cp $(abs_builddir)/packaging/deb/control $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN && \
	cp $(abs_builddir)/packaging/deb/postinst $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN && \
	cp $(abs_builddir)/packaging/deb/prerm $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN && \
	cp $(top_srcdir)/LICENSE $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN/copyright && \
	chmod 0755 $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN/postinst && \
	chmod 0755 $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN/prerm && \
	$(MAKE) $(AM_MAKEFLAGS) DESTDIR=$(abs_builddir)/$(DEB_PKG_FILE) install; \
	$(FAKEROOT) $(DPKG_DEB) --build $(abs_builddir)/$(DEB_PKG_FILE); \
	rm -rf $(DEB_PKG_FILE); \
	echo "$(DEB_PKG_FILE).deb" > $(abs_builddir)/_packaging
endif

if HAVE_RPM
rpm: all
	@cd $(abs_builddir) && \
	rm -rf $(PACKAGE_NAME)-$(PACKAGE_VERSION) && \
	mkdir $(PACKAGE_NAME)-$(PACKAGE_VERSION) && \
	$(MAKE) $(AM_MAKEFLAGS) install DESTDIR=$(abs_builddir)/$(PACKAGE_NAME)-$(PACKAGE_VERSION)/ && \
	cd $(abs_builddir)/packaging/rpm && \
	mkdir -p RPMS SRPMS BUILD SOURCES SPECS tmp && \
	cd $(abs_builddir)/packaging/rpm/SOURCES && \
	tar -zcv -C ../../../ -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(PACKAGE_NAME)-$(PACKAGE_VERSION)/ && \
	cd $(abs_builddir)/packaging/rpm && \
	rpmbuild -ba --define "_prefix /usr/local" --define "_topdir $(abs_builddir)/packaging/rpm" --define "_tmpdir $(abs_builddir)/packaging/rpm/tmp" SPECS/@PACKAGE_NAME@.spec && \
	cp $(abs_builddir)/packaging/rpm/RPMS/$(RPM_ARCH)/$(RPM_PKG_FILE) $(abs_builddir) && \
	echo "$(RPM_PKG_FILE)" > $(abs_builddir)/_packaging
endif
