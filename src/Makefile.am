## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = asaus.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(ASAUS_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = asaus

asaus_SOURCES = \
	main.cc \
	gui.cc \
	gui.h \
	executecode.cc \
	executecode.h \
	compilefrontend.cc \
	compilefrontend.h \
	terminal.cc \
	terminal.h \
	statusicon.cc \
	statusicon.h

asaus_CXXFLAGS = -std=c++11 \
	-std=gnu++11 

asaus_LDFLAGS = 

asaus_LDADD = $(ASAUS_LIBS)


EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)

