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

#include "radix-tree.h"

#define test_radix_get_bit(key, bit) (key[bit / 8] >> (7 - (bit % 8))) & 0x1

static gint
test_radix_cmp_bit (const guchar *key1,
                    const guchar *key2,
                    guint         bit)
{
  guint bit1 = test_radix_get_bit (key1, bit);
  guint bit2 = test_radix_get_bit (key2, bit);

  if (bit1 == bit2)
    return 0;
  if (bit1 > bit2)
    return 1;
  else
    return -1;
}

static gchar *
test_radix_tree_print_key (const guint8 *key,
                           guint         key_mask)
{
  gint i;
  GString *key_str;

  key_str = g_string_new ("");
  for (i = 0; i < key_mask; i ++)
    {
      if (i > 0 && i % 8 == 0)
        g_string_append (key_str, " ");

      g_string_append_printf (key_str, "%X", test_radix_get_bit (key, i));
    }

  return g_string_free (key_str, FALSE);
}

static gboolean
test_radix_tree_print_node (const guint8 *key,
                            guint         key_mask,
                            gpointer      value,
                            gpointer      data)
{
  gchar *key_str = test_radix_tree_print_key (key, key_mask);
  g_test_message ("[%s] mask: %d value: %s", key_str, key_mask, (gchar *)value);
  g_free (key_str);

  return FALSE;
}

static void
test_radix_tree_insert (void)
{
  RadixTree *tree = NULL;

  g_test_message ("Creating tree");
  tree = radix_tree_new ();
  g_assert (tree);

  guchar *key, *key2;
  guint key_mask;
  guint key_length;
  key_length = 2;
  key_mask = (key_length * 8);
  key = g_new0 (guchar, key_length);
  key[0] = 0b01010101;
  key[1] = 0b01010101;

  key2 = g_new0 (guchar, key_length);
  key2[0] = 0b10101010;
  key2[1] = 0b10101010;

  g_test_message ("Insert");
  gint i;
  for (i = 1; i <= key_mask; i ++)
    {
      radix_tree_insert (tree, key, i, "left");
      radix_tree_insert (tree, key2, i, "right");
    }

  key[1] = 0b10101010;
  key2[1] = 0b01010101;

  for (i = 9; i <= key_mask; i ++)
    {
      radix_tree_insert (tree, key, i, "left");
      radix_tree_insert (tree, key2, i, "right");
    }

  g_free (key);
  g_free (key2);

  radix_tree_unref (tree);
}

static void
test_radix_tree_search (void)
{
  RadixTree *tree = radix_tree_new ();
  GRand *rand = g_rand_new ();

  gint i;
  guint num = 1000000;

  guint32 key[num];
  guint key_mask[num];

  for (i = 0; i < num; i ++)
    {
      key[i] = g_rand_int (rand);
      key_mask[i] = g_rand_int_range (rand, 1, 32);

      radix_tree_insert (tree, (guint8 *)&key[i], key_mask[i], "ok");
    }

  for (i = 0; i < num; i ++)
    {
      guchar *found = (guchar *)radix_tree_exact_lookup (tree, (guint8 *)&key[i], key_mask[i]);
      g_assert_cmpstr (found, ==, "ok");
    }

  g_rand_free (rand);
  radix_tree_unref (tree);
}

static void
test_radix_tree_remove (void)
{
  RadixTree *tree = radix_tree_new ();
  GRand *rand = g_rand_new ();

  g_test_message ("nnodes = %u", radix_tree_nnodes (tree));

  gint i;
  guint num = 1000000;

  guint32 key[num];
  guint key_mask[num];

  for (i = 0; i < num; i ++)
    {
      key[i] = g_rand_int (rand);
      key_mask[i] = g_rand_int_range (rand, 1, 32);

      radix_tree_insert (tree, (guint8 *)&key[i], key_mask[i], "ok");
    }

  guint nnodes = radix_tree_nnodes (tree);
  g_test_message ("nnodes = %u", nnodes);

  gboolean success;
  for (i = 0; i < num; i ++)
    {
      success = radix_tree_remove (tree, (guint8 *)&key[i], key_mask[i]);
      if (success)
        nnodes --;
    }

  g_assert_cmpint (radix_tree_nnodes (tree), ==, 0);
  g_assert_cmpint (nnodes, ==, 0);

  g_rand_free (rand);
  radix_tree_unref (tree);
}

void
test_radix_tree_gb (void)
{
  guchar *key;
  guchar *key2;
  guint key_mask;
  guint key_length;

  key_length = 3;
  key_mask = (key_length * 8);
  key = g_new0 (guchar, key_length);
  key[0] = 0b01010101;
  key[1] = 0b01010101;
  key[2] = 0b01010101;
  key2 = g_memdup (key, key_length);

  gint i;
  for (i = 0; i < key_mask; i ++)
    {
      g_test_message ("%d", i);
      g_assert_cmpint (test_radix_get_bit (key, i), ==, (i % 2));
      g_assert_cmpint (test_radix_get_bit (key2, i), ==, (i % 2));
      g_assert_cmpint (test_radix_cmp_bit (key, key2, i), ==, 0);
    }

}

static void
test_radix_tree_foreach (void)
{
  RadixTree *tree = radix_tree_new ();
  GRand *rand = g_rand_new ();

  gint i;
  guint num = 1000000;

  guint32 key[num];
  guint key_mask[num];

  for (i = 0; i < num; i ++)
    {
      key[i] = g_rand_int (rand);
      key_mask[i] = g_rand_int_range (rand, 1, 32);

      radix_tree_insert (tree, (guint8 *)&key[i], key_mask[i], "ok");
    }

  radix_tree_foreach (tree, test_radix_tree_print_node, NULL);

  g_rand_free (rand);
  radix_tree_unref (tree);
}

gint
main (gint argc, gchar *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/radixtree/insert", test_radix_tree_insert);
  g_test_add_func ("/radixtree/search", test_radix_tree_search);
  g_test_add_func ("/radixtree/remove", test_radix_tree_remove);
  g_test_add_func ("/radixtree/get_bit", test_radix_tree_gb);
  g_test_add_func ("/radixtree/foreach", test_radix_tree_foreach);

  return g_test_run ();
}
