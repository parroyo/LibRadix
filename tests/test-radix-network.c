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

#include "config.h"

#include "radix-network.h"

#define test_radix_get_bit(key, bit) (key[bit / 8] >> (7 - (bit % 8))) & 0x1

static void
test_radix_network_insert (void)
{
  RadixNetwork *net = NULL;
  GInetAddressMask *addr = NULL;
  GInetAddressMask *addr2 = NULL;
  GInetAddressMask *addr_res = NULL;

  g_test_message ("Creating network");
  net = radix_network_new ();
  g_assert (RADIX_IS_NETWORK (net));
  g_assert_cmpint (radix_network_size (net), ==, 0);

  addr = g_inet_address_mask_new_from_string ("10.0.0.0/32", NULL);
  addr2 = g_inet_address_mask_new_from_string ("::10.0.0.0", NULL);

  g_assert (addr);
  g_assert (addr2);

  g_test_message ("Insert IPV4");
  radix_network_insert (net, addr);
  g_assert_cmpint (radix_network_size (net), ==, 1);

  g_test_message ("Insert IPV6");
  radix_network_insert (net, addr2);
  g_assert_cmpint (radix_network_size (net), ==, 2);

  g_test_message ("Exact Lookup IPV4");
  addr_res = radix_network_lookup (net, addr);
  g_assert (addr_res != NULL);

  g_test_message ("Exact Lookup IPV6");
  addr_res = radix_network_lookup (net, addr2);
  g_assert (addr_res != NULL);

  g_test_message ("Remove IPV4");
  g_assert (radix_network_remove (net, addr));
  g_assert_cmpint (radix_network_size (net), ==, 1);

  g_test_message ("!Exact Lookup IPV4");
  addr_res = radix_network_lookup (net, addr);
  g_assert (addr_res == NULL);

  g_test_message ("Remove IPV6");
  g_assert (radix_network_remove (net, addr2));
  g_assert_cmpint (radix_network_size (net), ==, 0);

  g_test_message ("!Exact Lookup IPV6");
  addr_res = radix_network_lookup (net, addr2);
  g_assert (addr_res == NULL);

  g_object_unref (addr);
  g_object_unref (addr2);

  g_object_unref (net);
}

static void
test_radix_network_lookup (void)
{
  RadixNetwork *net = NULL;
  GInetAddressMask *addr_net1 = NULL;
  GInetAddressMask *addr_net2 = NULL;
  GInetAddressMask *addr1 = NULL;
  GInetAddressMask *addr2 = NULL;
  GInetAddressMask *addr_res = NULL;

  g_test_message ("Creating network");
  net = radix_network_new ();
  g_assert (RADIX_IS_NETWORK (net));
  g_assert_cmpint (radix_network_size (net), ==, 0);

  addr_net1 = g_inet_address_mask_new_from_string ("192.168.0.0/16", NULL);
  addr_net2 = g_inet_address_mask_new_from_string ("192.168.1.0/24", NULL);

  addr1 = g_inet_address_mask_new_from_string ("192.168.100.1/32", NULL);
  addr2 = g_inet_address_mask_new_from_string ("192.168.1.1/32", NULL);

  g_assert (addr_net1);
  g_assert (addr_net2);
  g_assert (addr1);
  g_assert (addr2);

  g_test_message ("Insert nets");
  radix_network_insert (net, addr_net1);
  g_assert_cmpint (radix_network_size (net), ==, 1);

  radix_network_insert (net, addr_net2);
  g_assert_cmpint (radix_network_size (net), ==, 2);

  g_test_message ("Lookup");
  addr_res = radix_network_lookup (net, addr1);
  g_assert (addr_res == addr_net1);
  addr_res = radix_network_lookup (net, addr2);
  g_assert (addr_res == addr_net2);

  g_test_message ("Insert addrs");
  radix_network_insert (net, addr1);
  radix_network_insert (net, addr2);
  g_assert_cmpint (radix_network_size (net), ==, 4);

  g_test_message ("Lookup 2");
  addr_res = radix_network_lookup (net, addr1);
  g_assert (addr_res == addr1);
  addr_res = radix_network_lookup (net, addr2);
  g_assert (addr_res == addr2);

  g_object_unref (addr_net1);
  g_object_unref (addr_net2);
  g_object_unref (addr1);
  g_object_unref (addr2);

  g_object_unref (net);
}

gint
main (gint argc, gchar *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/radixtree/insert", test_radix_network_insert);
  g_test_add_func ("/radixtree/lookup", test_radix_network_lookup);

  return g_test_run ();
}
