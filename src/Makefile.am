## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir =$(pkgdatadir)/ui
ui_DATA = ui/asaus.ui \
	ui/downleft.png \
	ui/downright.png \
	ui/topleft.png \
	ui/topright.png \
	ui/closedialog.ui \
	ui/asaus.png


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
	statusicon.h \
	closingdialog.cc \
	closingdialog.h

asaus_CXXFLAGS = -std=c++0x \
	-std=gnu++0x 

asaus_LDFLAGS = 

asaus_LDADD = $(ASAUS_LIBS)


EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)


SUBDIRS = \
	applications
