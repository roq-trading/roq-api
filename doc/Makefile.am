ACLOCAL_AMFLAGS = -I m4

SUBDIRS = .

if HAVE_DOXYGEN

all-local: doxygen-build.stamp

doxygen-build.stamp: Doxyfile
	doxygen
	touch $@

install-data-local:
	$(INSTALL) -d $(DESTDIR)$(docdir)/html/search
	for f in `find html -type f \! -name "installdox"`; do	\
		$(INSTALL_DATA) $$f $(DESTDIR)$(docdir)/$$f;	\
	done
	$(INSTALL) -d $(DESTDIR)$(docdir)/xml
	for f in `find xml -type f`; do	\
		$(INSTALL_DATA) $$f $(DESTDIR)$(docdir)/$$f;	\
	done

uninstall-local:
	$(RM) -r $(DESTDIR)$(docdir)/html
	$(RM) -r $(DESTDIR)$(docdir)/xml


CLEANFILES = doxygen-build.stamp

clean-local:
	rm -rf {html,xml}

endif
