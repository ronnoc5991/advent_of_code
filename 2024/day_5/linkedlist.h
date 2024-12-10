#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define get_next_node(node) (node->next)
#define set_next_node(node, next_node) (node->next = next_node)
#define get_node_payload(node) (node->payload)
#define set_node_payload(node, payload) (node->payload = payload)

typedef struct listNode {
	void *payload;
	struct listNode *next;
} ListNode;

ListNode *create_list_node(void *payload);

void free_list(ListNode *list, void(*free_payload)(void *));

ListNode *insert_list_node(ListNode *list, ListNode *node);

ListNode *append_list_node(ListNode *list, ListNode *node);

ListNode *get_nth_node(ListNode *list, int n);

#endif
