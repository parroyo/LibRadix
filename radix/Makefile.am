MAINTAINERCLEANFILES = \
				Makefile.in

pkgconfigdir = $(libdir)/pkgconfig

library_includedir=$(includedir)/@PRJ_API_NAME@
library_include_HEADERS = \
	radix-tree.h \
	radix-network.h

AM_CFLAGS = $(GLIB_CFLAGS) -Wall \
				-DPKGDATADIR="\"$(pkgdatadir)\"" \
				-DENABLE_TESTS="\"$(enable_tests)\""

if ENABLE_DEBUG
AM_CFLAGS += -Werror -g3 -O0 -ggdb
else
AM_CFLAGS += -DG_DISABLE_ASSERT -DG_DISABLE_CHECKS
endif

# libraries
lib_LTLIBRARIES = lib@PRJ_API_NAME@.la

# libradix
source_c = \
	radix-tree.c \
	radix-network.c

source_h = \
	radix-tree.h	\
	radix-network.h

source_h_priv =


lib@PRJ_API_NAME@_la_LIBADD = \
				$(GLIB_LIBS) \
				-lm

lib@PRJ_API_NAME@_la_CFLAGS = \
				$(AM_CFLAGS)

lib@PRJ_API_NAME@_la_LDFLAGS = \
				-version-info 0:1:0 \
				-no-undefined

lib@PRJ_API_NAME@_la_SOURCES = \
				$(source_c) \
				$(source_h) \
				$(source_h_priv)
