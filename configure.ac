#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.62])

m4_define([prj_name],       [radix])
m4_define([prj_short_name], [radix])
m4_define([prj_home],       [http://url])

# package version number (not shared library version)
# odd micro numbers indicate in-progress development
# even micro numbers indicate released versions
m4_define([prj_version_major], [0])
m4_define([prj_version_minor], [0])
m4_define([prj_version_micro], [1])

m4_define([prj_version],
          [prj_version_major.prj_version_minor.prj_version_micro])
m4_define([prj_api_version],
          [prj_version_major.prj_version_minor])

AC_INIT([prj_name],
        [prj_version],
        [prj_home])

AC_CONFIG_HEADERS(config.h)
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE

PRJ_NAME="prj_short_name"
AC_SUBST(PRJ_NAME)

PRJ_VERSION="prj_version"
AC_SUBST(PRJ_VERSION)

PRJ_API_VERSION="prj_api_version"
AC_SUBST(PRJ_API_VERSION)

PRJ_API_NAME="prj_short_name-prj_api_version"
AC_SUBST(PRJ_API_NAME)


# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O

# enable pkg-config
PKG_PROG_PKG_CONFIG

# Required libraries
GLIB_REQUIRED=2.28.0

PKG_CHECK_MODULES(GLIB, gio-2.0 >= $GLIB_REQUIRED
                        glib-2.0 >= $GLIB_REQUIRED
                        gthread-2.0 >= $GLIB_REQUIRED
                        gobject-2.0 >= $GLIB_REQUIRED)

# GTK Doc
GTK_DOC_CHECK([1.11])
AM_CONDITIONAL([BUILD_GTK_DOC], [test "x$enable_gtk_doc" = "xyes" || test ! -f "autogen.sh"])

# Silent build
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

# Tests
AC_ARG_ENABLE(tests,
              AS_HELP_STRING([--enable-tests[=@<:@no/yes@:>@]],
              [Enable automated unit and functional tests [default=yes]]),,
              [enable_tests=yes])

AM_CONDITIONAL(ENABLE_TESTS, test x"${enable_tests}" = x"yes")

# Debug
AC_ARG_ENABLE(debug,
        AS_HELP_STRING([--enable-debug[=@<:@no/yes@:>@]],
                [Enable debug mode by adding -ggdb, -g3, -O0 and -Werror to CFLAGS [default=no]]),,
                [enable_debug=no])

AM_CONDITIONAL(ENABLE_DEBUG, test x"${enable_debug}" = x"yes")

RADIX_CFLAGS="$GLIB_CFLAGS"
RADIX_LIBS="$GLIB_LIBS"

AC_SUBST(RADIX_CFLAGS)
AC_SUBST(RADIX_LIBS)

AC_CONFIG_FILES([Makefile
                 radix/Makefile
                 doc/Makefile
                 doc/reference/Makefile
                 tests/Makefile])
AC_OUTPUT

echo ""
echo "   Lib Radix $VERSION"
echo " ===================="
echo ""
echo "  Install prefix:           ${prefix}"
echo "  Build API documentation:  ${enable_gtk_doc}"
echo "  Enable debug mode:        ${enable_debug}"
echo "  Enable automated tests:   ${enable_tests}"
echo ""