## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

asausdocdir = ${datarootdir}/doc/asaus
asausdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS\
	TODO


EXTRA_DIST = $(asausdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(asausdocdir)
