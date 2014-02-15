#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct ll_node {
  struct ll_node * next, * prev;
} ll_node_t;

/*
 * All functions can handle nulls as *prev or *next in the head properly
 */

extern void linkedlist_insert_after (ll_node_t * head, ll_node_t * node);
extern void linkedlist_insert_before(ll_node_t * head, ll_node_t * node);
//DOES NOT free()
extern void linkedlist_remove(ll_node_t * to_remove);

#endif
