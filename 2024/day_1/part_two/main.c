#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
	int value;
	struct ListNode *next;
} ListNode;

ListNode *create_list_node(int value) {
	ListNode *node = malloc(sizeof(ListNode));
	node->value = value;
	node->next = NULL;
	return node;
}

// returns the new 'head' of the list
// insert each item at the head of the list
ListNode *insert_list_node(ListNode *list, ListNode *node) {
	node->next = list;
	return node;
}

void free_list(ListNode *node) {
	if (node == NULL) {
		return;
	}

	free_list(node->next);
	free(node);
}

#define TABLE_SIZE 10

typedef struct TableNode {
	int key;
	int value;
	struct TableNode *next;
} TableNode;

TableNode *table[TABLE_SIZE] = { NULL };

TableNode *create_table_node(int key, int value) {
	TableNode *node = malloc(sizeof(TableNode));
	node->key = key;
	node->value = value;
	node->next = NULL;
	return node;
}

void free_table_list(TableNode *node) {
	if (node == NULL) {
		return;
	}

	free_table_list(node->next);
	free(node);
}

void free_table() {
	for (int i = 0; i < TABLE_SIZE; i++) {
		free_table_list(table[i]);
	}
}

int hash(int key) {
	int value;
	while ((value = (key % TABLE_SIZE)) > TABLE_SIZE) {}
	printf("hashed: %d\n", value);
	return value;
}

// insert the node into the table
void insert(TableNode *node) {
	int index = hash(node->key);
	node->next = table[index];
	table[index] = node;
}

// given a key, get the stored value? or return NULL
TableNode *get(int key) {
	int index = hash(key);

	for (TableNode *p = table[index]; p; p = p->next) {
		if (p->key == key) {
			return p;
		}
	}

	return NULL;
}

void set(int key, int value) {
	TableNode *existing_node = get(key);

	if (existing_node != NULL) {
		existing_node->value = value;
		return;
	}

	insert(create_table_node(key, value));
}

int main(int argc, char **argv) {
	char *executable_name = argv[0];
	char *input_filename = argv[1];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input file name>\n", executable_name);
		return 1;
	}
	
	FILE *fp = fopen(input_filename, "r");

	if (fp == NULL) {
		fprintf(stderr, "Unable to open file: %s\n", input_filename);
		return 1;
	}

	// we do still want the first list
	ListNode *list_one = NULL;

	int num_items_assigned = 0;
	int line_number = 0;
	int col_one_value;
	int col_two_value;
	
	while((num_items_assigned = (fscanf(fp, "%d   %d", &col_one_value, &col_two_value))) != EOF) {
		line_number++;

		if (num_items_assigned != 2) {
			fprintf(stderr, "failed to parse two values from line %d\n", line_number);
			free_list(list_one);
			return 1;
		}

		list_one = insert_list_node(list_one, create_list_node(col_one_value));
		TableNode *existing_node = get(col_two_value);

		if (existing_node == NULL) {
			set(col_two_value, 1);
		} else {
			set(col_two_value, existing_node->value + 1);
		}
	}

	fclose(fp);

	unsigned long int product = 0;

	for (ListNode *p_one = list_one; p_one; p_one = p_one->next) {
		// lookup how many times this value appears
		TableNode *existing_node = get(p_one->value);

		if (existing_node == NULL) {
			continue;
		} else {
			product += p_one->value * existing_node->value;
		}
	}

	printf("Total product: %lu\n", product);

	free_list(list_one);
}
