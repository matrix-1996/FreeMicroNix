#ifndef _DATASTRUCT_LIST_H
#define _DATASTRUCT_LIST_H

struct list;
struct list_node;

typedef struct list
{
	struct list_node *head;
	struct list_node *tail;
} list_t;

typedef struct list_node
{
	struct list_node *next;
	struct list_node *prev;
	struct list *list;
	int priority;
} list_node_t;

#define LIST_INIT {0,0}

void list_push_head(list_t *list, list_node_t *node);
void list_push_tail(list_t *list, list_node_t *node);
void list_push_priority( list_t *list, list_node_t *node, int priority);
list_node_t *list_pop_head( list_t *list);
list_node_t *list_pop_tail( list_t *list);
void list_remove( list_node_t *node);


#endif