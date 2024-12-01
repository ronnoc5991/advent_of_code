#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int value;
	struct Node *next;
} Node;

Node *create_node(int value) {
	Node *node = malloc(sizeof(Node));
	node->value = value;
	node->next = NULL;
	return node;
}

// returns the new 'head' of the list
Node *insert_node(Node *list, Node *node) {
	if (list == NULL || node->value < list->value) {
		node->next = list;
		return node;
	}

	// no explicit test case... eventually p_two will be NULL...
	for (Node *p_one = list, *p_two = list->next; ; p_one = p_one->next, p_two = p_two->next) {
		if (p_two == NULL || node->value < p_two->value) {
			node->next = p_two;
			p_one->next = node;
			return list;
		}
	}
}

void free_list(Node *node) {
	if (node == NULL) {
		return;
	}

	free_list(node->next);
	free(node);
}

// write a function to 'populate' the lists?

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

	Node *list_one = NULL;
	Node *list_two = NULL;

	int num_items_assigned = 0;
	int line_number = 0;
	int col_one_value;
	int col_two_value;
	
	while((num_items_assigned = (fscanf(fp, "%d   %d", &col_one_value, &col_two_value))) != EOF) {
		line_number++;

		if (num_items_assigned != 2) {
			fprintf(stderr, "failed to parse two values from line %d\n", line_number);
			free_list(list_one);
			free_list(list_two);
			return 1;
		}

		list_one = insert_node(list_one, create_node(col_one_value));
		list_two = insert_node(list_two, create_node(col_two_value));
	}

	fclose(fp);

	unsigned int total_difference = 0;

	for (Node *p_one = list_one, *p_two = list_two; p_one && p_two; p_one = p_one->next, p_two = p_two->next) {
		int difference = p_one->value - p_two->value;

		if (difference < 0) {
			total_difference += difference * -1;
		} else {
			total_difference += difference;
		}
	}

	printf("Total difference: %d\n", total_difference);

	free_list(list_one);
	free_list(list_two);
}
