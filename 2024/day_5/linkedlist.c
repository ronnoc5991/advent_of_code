#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>

ListNode *create_list_node(void *payload) {
	ListNode *node = malloc(sizeof(ListNode));
	node->payload = payload;
	node->next = NULL;
	return node;
}

void free_list_node(ListNode *node, void(*free_payload)(void *)) {
	free_payload(node->payload);
	free(node);
}

void free_list(ListNode *list, void(*free_payload)(void *)) {
	if (list == NULL) {
		return;
	}

	free_list(list->next, free_payload);
	free_list_node(list, free_payload);
}

ListNode *insert_list_node(ListNode *list, ListNode *node) {
	node->next = list;
	return node;
}

ListNode *append_list_node(ListNode *list, ListNode *node) {
	if (list == NULL) {
		return node;
	}

	ListNode *p = list;

	while(p) {
		ListNode *next_node = get_next_node(p);

		if (next_node == NULL) {
			set_next_node(p, node);
			break;
		}

		p = get_next_node(p);
	}

	return list;
}

ListNode *get_nth_node(ListNode *list, int n) {
	if (n == 0 || list == NULL) {
		return list;
	}

	return get_nth_node(get_next_node(list), n - 1);
}
