dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(asaus, 0.5)


AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])


AC_PROG_CXX([g++ clang++])

AC_CONFIG_HEADERS([config.h])



LT_INIT



PKG_CHECK_MODULES(ASAUS, gtkmm-3.0 vte-2.91[ ])




AC_CONFIG_FILES([
Makefile
src/Makefile
src/applications/Makefile
src/ui/Makefile

])

AC_OUTPUT
