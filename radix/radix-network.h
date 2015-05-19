/*
 * Copyright (C) 2014  Pablo Arroyo loma
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __RADIX_NETWORK_H__
#define __RADIX_NETWORK_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define RADIX_TYPE_NETWORK            (radix_network_get_type ())
#define RADIX_NETWORK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), RADIX_TYPE_NETWORK, RadixNetwork))
#define RADIX_NETWORK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), RADIX_TYPE_NETWORK, RadixNetworkClass))
#define RADIX_IS_NETWORK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), RADIX_TYPE_NETWORK))
#define RADIX_IS_NETWORK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), RADIX_TYPE_NETWORK))
#define RADIX_NETWORK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), RADIX_TYPE_NETWORK, RadixNetworkClass))

typedef struct _RadixNetwork        RadixNetwork;
typedef struct _RadixNetworkClass   RadixNetworkClass;
typedef struct _RadixNetworkPrivate RadixNetworkPrivate;

struct _RadixNetwork
{
  GObject parent;

  /*< private >*/
  RadixNetworkPrivate *priv;
};

/**
 * RadixNetworkClass:
 **/
struct _RadixNetworkClass
{
  GObjectClass parent_class;
};

GType                           radix_network_get_type                  (void) G_GNUC_CONST;
RadixNetwork *                  radix_network_new                       (void);
RadixNetwork *                  radix_network_ref                       (RadixNetwork       *network);
void                            radix_network_unref                     (RadixNetwork       *network);
void                            radix_network_insert                    (RadixNetwork       *network,
                                                                         GInetAddressMask   *address);
gboolean                        radix_network_remove                    (RadixNetwork       *network,
                                                                         GInetAddressMask   *address);
GInetAddressMask *              radix_network_lookup                    (RadixNetwork       *network,
                                                                         GInetAddressMask   *address);
GInetAddressMask *              radix_network_exact_lookup              (RadixNetwork       *network,
                                                                         GInetAddressMask   *address);
guint                           radix_network_size                      (RadixNetwork       *network);


G_END_DECLS

#endif /* __RADIX_NETWORK_H__ */
