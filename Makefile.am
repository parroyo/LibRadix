
SUBDIRS = radix

if ENABLE_TESTS
SUBDIRS += tests
endif

if BUILD_GTK_DOC
SUBDIRS += doc
endif

DIST_SUBDIRS = \
  radix

EXTRA_DIST = \
  autogen.sh \
  m4/introspection.m4

ACLOCAL_AMFLAGS = -I m4

CLEANFILES = *~

MAINTAINERCLEANFILES = \
				Makefile.in aclocal.m4 configure config.guess config.sub \
				depcomp install-sh ltmain.sh missing mkinstalldirs config.h.in \
				stamp-h.in compile

DISTCLEANFILES = \
				cscope.files cscope.out

cscope.files:
				find src -name '*.[ch]' > $@

cscope.out: cscope.files
				cscope -b
