AUTOGEN_H = \
	api.h
CLEANFILES = $(AUTOGEN_H)
BUILT_SOURCES = \
	$(CLEANFILES) \
	.gitignore
pkgincludedir=$(includedir)/roq/fbs
pkginclude_HEADERS = \
	$(AUTOGEN_H)

.gitignore: Makefile.am
	echo $(AUTOGEN_H) | tr " " "\n" | sort > $@

$(AUTOGEN_H): %.h: ../../../schema/fbs/%.fbs
	$(FLATC) \
                --binary \
                --cpp \
                --cpp-std c++17 \
                --gen-all \
                --filename-suffix '' \
                $<
