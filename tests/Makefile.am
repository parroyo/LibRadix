MAINTAINERCLEANFILES = \
	Makefile.in

AM_CFLAGS = \
	$(GLIB_CFLAGS) \
	-I$(top_srcdir)/@PRJ_NAME@

if ENABLE_DEBUG
AM_CFLAGS += -Werror -g3 -O0 -ggdb
endif

LDADD = \
	$(top_builddir)/@PRJ_NAME@/lib@PRJ_API_NAME@.la \
	$(GLIB_LIBS)

noinst_PROGRAMS = \
	test-radix-tree \
	test-radix-network

TESTS = \
	test-radix-tree \
	test-radix-network

test_radix_tree_SOURCES = test-radix-tree.c

test_radix_network_SOURCES = test-radix-network.c
