Lib Radix - A Glib-based Radix tree implementation
==================================================

Lib Radix is an non-recursive implementation of radix tree using glib.
It Also provides the RadixNetwork class, an interface for handling
inet addresses using radix tree.
Lib Radix provides methods for insert, remove, lookup and traverse nodes

 * [Radix Tree][radix-tree]
 * [GLib and GObject][glib]


REQUIREMENTS
------------
In order to build Lib Radix you will need:

 * pkg-config
 * libtool
 * automake
 * gtk-doc-tools
 * GLib, GIO ≥ 2.32

INSTALLATION
-------------
To build Lib Radix just run:

    $ ./autogen.sh
    $ make
    $ sudo make install

AUTHOR, COPYRIGHT AND LICENSING
-------------------------------
Lib Radix has been written by Pablo Arroyo Loma

Lib Radix is released under the terms of the GNU Lesser General Public License,
either version 3 or (at your option) any later version.

See the file COPYING for details.

Copyright (C) 2014  Pablo Arroyo Loma

[radix-tree]: http://en.wikipedia.org/wiki/Radix_tree
[glib]: http://www.gtk.org