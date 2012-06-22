## Process this file with automake to produce Makefile.in

## Created by Anjuta

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

uninstall-local:
	-rm -r $(pkgdatadir)

SUBDIRS = \
	applications \
	ui
