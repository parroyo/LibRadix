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

/**
 * SECTION:radix-network
 * @short_description: Object that store Inet addresses using radix tree.
 *
 * RadixNetwork objects store Inet addresses using radix tree.
 *
 **/

#include "config.h"

#include "radix-network.h"

#include "radix-tree.h"

#define RADIX_NETWORK_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), \
                                        RADIX_TYPE_NETWORK,                 \
                                        RadixNetworkPrivate))

/* Private data */
struct _RadixNetworkPrivate
{
  RadixTree     *tree;
};

static void     radix_network_class_init              (RadixNetworkClass *class);
static void     radix_network_init                    (RadixNetwork *self);
static void     radix_network_finalize                (GObject *obj);
static void     radix_network_dispose                 (GObject *obj);

G_DEFINE_TYPE (RadixNetwork, radix_network, G_TYPE_OBJECT)

static void
radix_network_class_init (RadixNetworkClass *class)
{
  GObjectClass *obj_class;

  obj_class = G_OBJECT_CLASS (class);

  obj_class->dispose = radix_network_dispose;
  obj_class->finalize = radix_network_finalize;

  /* add private structure */
  g_type_class_add_private (obj_class, sizeof (RadixNetworkPrivate));
}

static void
radix_network_init (RadixNetwork *self)
{
  RadixNetworkPrivate *priv;

  priv = RADIX_NETWORK_GET_PRIVATE (self);
  self->priv = priv;

  priv->tree = radix_tree_new_with_destroy_func (g_object_unref);
}

static void
radix_network_dispose (GObject *obj)
{
  G_OBJECT_CLASS (radix_network_parent_class)->dispose (obj);
}

static void
radix_network_finalize (GObject *obj)
{
  RadixNetwork *self = RADIX_NETWORK (obj);

  radix_tree_unref (self->priv->tree);

  G_OBJECT_CLASS (radix_network_parent_class)->finalize (obj);
}

/*
 * PUBLIC API
 */

/**
 * radix_network_new:
 *
 * Creates a new #RadixNetwork.
 *
 * Returns: (transfer none): a new #RadixNetwork
 **/
RadixNetwork *
radix_network_new (void)
{
  return g_object_new (RADIX_TYPE_NETWORK, NULL);
}

/**
 * radix_network_insert:
 * @network: a #RadixNetwork.
 * @address: a #GInetAddressMask.
 *
 * Inserts a #GInetAddressMask into a #RadixNetwork. If the given key
 * already exists in the #RadixNetwork its corresponding value is set to the
 * new value.
 **/
void
radix_network_insert (RadixNetwork     *network,
                      GInetAddressMask *address)
{
  GInetAddress *address_inet;
  const guint8 *address_bytes;
  guint mask_length;

  g_return_if_fail (network != NULL);
  g_return_if_fail (address != NULL);

  address_inet = g_inet_address_mask_get_address (address);
  address_bytes = g_inet_address_to_bytes (address_inet);
  mask_length = g_inet_address_mask_get_length (address);

  address = g_object_ref (address);
  radix_tree_insert (network->priv->tree, address_bytes, mask_length, address);
}

/**
 * radix_network_remove:
 * @network: a #RadixNetwork.
 * @address: a #GInetAddressMask.
 *
 * Removes the #GInetAddressMask from the #RadixNetwork
 *
 * Returns: %TRUE if the #GInetAddressMask exits in the #RadixNetwork
 * and it is removed, %FALSE otherwise.
 **/
gboolean
radix_network_remove (RadixNetwork     *network,
                      GInetAddressMask *address)
{
  GInetAddress *address_inet;
  const guint8 *address_bytes;
  guint mask_length;

  g_return_val_if_fail (network != NULL, FALSE);
  g_return_val_if_fail (address != NULL, FALSE);

  address_inet = g_inet_address_mask_get_address (address);
  address_bytes = g_inet_address_to_bytes (address_inet);
  mask_length = g_inet_address_mask_get_length (address);

  return radix_tree_remove (network->priv->tree, address_bytes, mask_length);
}

/**
 * radix_network_lookup:
 * @network: a #RadixNetwork.
 * @address: a #GInetAddressMask.
 *
 * Lookup the best matching #GInetAddressMask in the #RadixNetwork
 *
 * Returns: The found #GInetAddressMask object or %NULL if the @address
 * is not contained in the #RadixNetwork.
 **/
GInetAddressMask *
radix_network_lookup (RadixNetwork     *network,
                      GInetAddressMask *address)
{
  GInetAddress *address_inet;
  const guint8 *address_bytes;
  guint mask_length;

  g_return_val_if_fail (network != NULL, NULL);
  g_return_val_if_fail (address != NULL, NULL);

  address_inet = g_inet_address_mask_get_address (address);
  address_bytes = g_inet_address_to_bytes (address_inet);
  mask_length = g_inet_address_mask_get_length (address);

  return radix_tree_lookup (network->priv->tree, address_bytes, mask_length, NULL);
}

/**
 * radix_network_exact_lookup:
 * @network: a #RadixNetwork.
 * @address: a #GInetAddressMask.
 *
 * Lookup the exact matching #GInetAddressMask in the #RadixNetwork
 *
 * Returns: The found #GInetAddressMask object or %NULL if the @address
 * doesn't exist in the #RadixNetwork.
 **/
GInetAddressMask *
radix_network_exact_lookup (RadixNetwork     *network,
                            GInetAddressMask *address)
{
  GInetAddress *address_inet;
  const guint8 *address_bytes;
  guint mask_length;

  g_return_val_if_fail (network != NULL, NULL);
  g_return_val_if_fail (address != NULL, NULL);

  address_inet = g_inet_address_mask_get_address (address);
  address_bytes = g_inet_address_to_bytes (address_inet);
  mask_length = g_inet_address_mask_get_length (address);

  return radix_tree_exact_lookup (network->priv->tree, address_bytes, mask_length);
}

/**
 * radix_network_size:
 * @network: a #RadixNetwork.
 *
 * Gets the number of elements in a #RadixNetwork.
 *
 * Returns: the number of elements in the #RadixNetwork.
 **/
guint
radix_network_size (RadixNetwork *network)
{
  g_return_val_if_fail (network != NULL, 0);

  return radix_tree_nnodes (network->priv->tree);
}
