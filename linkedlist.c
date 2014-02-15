#include "linkedlist.h"

#include "log.h"

void linkedlist_insert_after (ll_node_t * head, ll_node_t * node) {
  ll_node_t * next;
  next = head->next;
  node->next = next;
  head->next = node;
  swiftsure_log(DEBUG, "%x %x\n", head, head->next);
  if (next) {
    next->prev = node;
  }
}

void linkedlist_insert_before(ll_node_t * head, ll_node_t * node) {
  ll_node_t * prev;
  prev = head->prev;
  node->prev = prev;
  node->next = head;
  if (prev) {
    prev->next = node;
  }
}

void linkedlist_remove(ll_node_t * to_remove) {
  ll_node_t * prev, * next;
  prev = to_remove->prev;
  next = to_remove->next;
  prev->next = next;
  next->prev = prev;
}
