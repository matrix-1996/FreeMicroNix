#include <datastruct/list.h>

void list_push_head(list_t *list, list_node_t *node)
{
	node->next = list->head;
	node->prev = 0;
	node->priority = 0;

	if (list->head)
	{
		list->head->prev = node;	
	} 

	if (!list->tail)
	{
		list->tail = node;
	}

	node->list = list;
}

void list_push_tail(list_t *list, list_node_t *node)
{
	node->prev = list->tail;
	node->next = 0;
	node->priority = 0;

	if (list->tail)
	{
		list->tail->next = node;
	}

	if (!list->head)
	{
		list->head = node;
	}
	node->list = list;
}


void list_push_priority( list_t *list, list_node_t *node, int priority)
{
	list_node_t *n;
	int i = 0;

	if (!list->head)
	{
		list_push_head(list, node);
		return;
	}

	for (n = list->head; n; n = n->next)
	{
		if (priority > n->priority || i > 5000)
		{
			node->next = n;
			node->prev = n->prev;
			node->priority = priority;

			if (n->prev)
			{
				n->prev->next = node;
			}
			else
			{
				list->head = node;
			}

			n->prev = node;
			node->list = list;
			return;

		}
		i++;
	}
	list_push_tail(list, node);
}

list_node_t *list_pop_head( list_t *list)
{
	list_node_t *result = list->head;

	if (list->head)
	{
		list->head = list->head->next;
		
		if (list->head)
		{
			list->head->prev = 0;
		}
		
		if (!list->head)
		{
			list->tail = 0;
		}

		result->next = result->prev = 0;
		result->list = 0;
	}

	return result;
}

list_node_t *list_pop_tail( list_t *list)
{
	list_node_t *result = list->tail;

	if (list->tail)
	{
		list->tail = list->tail->prev;
		
		if (list->tail)
		{
			list->tail->next = 0;
		}
		
		if (!list->tail)
		{
			list->head = 0;
		}

		result->next = result->prev = 0;
		result->list = 0;
	}

	return result;	
}

void list_remove( list_node_t *node)
{
	if (!node->list) 
	{
		return;
	}

	if (node->list->head == node)
	{
		list_pop_head(node->list);
		return;
	}

	if (node->list->tail == node)
	{
		list_pop_tail(node->list);
		return;
	}

	node->next->prev = node->prev;
	node->prev->next = node->next;

	node->next = node->prev = 0;
	node->list = 0;
}
