#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

static uint16_t values[256];

/*
 * Combine two list with two head
 */
static void combine(struct list_head *head1, struct list_head *head2) {
  struct listitem *item = NULL, *is = NULL;
  struct list_head tmp;
  INIT_LIST_HEAD(&tmp);

  list_for_each_entry_safe(item, is, head1, list) {
    struct listitem *Item = NULL, *Is = NULL;
    list_for_each_entry_safe(Item, Is, head2, list) {
      if (Item->i > item->i)
        break;
      else {
        list_del(&Item->list);
        list_add_tail(&Item->list, &tmp);
      }
    }
    list_del(&item->list);
    list_add_tail(&item->list, &tmp);
  }

  if (list_empty(head1))
    list_splice_tail(head2, &tmp);
  else
    list_splice_tail(head1, &tmp);

  INIT_LIST_HEAD(head1);
  list_splice_tail_init(&tmp, head1);
}

static void Mergesort(struct list_head *head) {
  if (list_is_singular(head))
    return;

  struct list_head *tmp1, *tmp2, mid, *is = NULL;
  tmp1 = head->prev;
  INIT_LIST_HEAD(&mid);
  struct listitem *ptr = NULL;

  struct list_head *tmp_del;

  /*
   * Find the mid element of the list
   * Use two-way linear search
   * mid list
   */

  int count = 0;
  list_for_each(tmp1, head) { count++; }
  count /= 2;

  int i = 0;
  list_for_each(tmp1, head) {
    i++;
    if (i == count)
      tmp2 = tmp1;
  }
  list_cut_position(&mid, head, tmp2);

  Mergesort(head);
  Mergesort(&mid);
  combine(head, &mid);
}

int main() {
  struct list_head testlist;
  struct listitem *item = NULL, *is = NULL;
  size_t i;

  random_shuffle_array(values, (uint16_t)ARRAY_SIZE(values));

  INIT_LIST_HEAD(&testlist);

  assert(list_empty(&testlist));

  for (i = 0; i < ARRAY_SIZE(values); i++) {
    item = (struct listitem *)malloc(sizeof(*item));
    assert(item);
    item->i = values[i];
    list_add_tail(&item->list, &testlist);
  }

  assert(!list_empty(&testlist));

  qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);

  Mergesort(&testlist);

  i = 0;
  list_for_each_entry_safe(item, is, &testlist, list) {
    assert(item->i == values[i]);
    list_del(&item->list);
    free(item);
    i++;
  }

  assert(i == ARRAY_SIZE(values));
  assert(list_empty(&testlist));

  printf("OK\n");

  return 0;
}