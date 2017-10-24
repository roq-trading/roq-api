SUBDIRS=include

.PHONY: lint

lint:
	cpplint --linelength=120 --root=subdir=src/ --recursive include/

if HAVE_DEB
deb: all
	@cd $(abs_builddir) && \
	mkdir -p $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN && \
	cp $(abs_builddir)/packaging/debian/control $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN && \
	cp $(abs_builddir)/packaging/debian/postinst $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN && \
	cp $(abs_builddir)/packaging/debian/prerm $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN && \
	chmod 0755 $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN/postinst && \
	chmod 0755 $(abs_builddir)/$(DEB_PKG_FILE)/DEBIAN/prerm && \
	$(MAKE) $(AM_MAKEFLAGS) DESTDIR=$(abs_builddir)/$(DEB_PKG_FILE) install; \
	$(FAKEROOT) $(DPKG_DEB) --build $(abs_builddir)/$(DEB_PKG_FILE); \
	rm -rf $(DEB_PKG_FILE); \
	echo "$(DEB_PKG_FILE).deb" > $(abs_builddir)/_packaging
endif
