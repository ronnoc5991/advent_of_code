#include "linkedlist.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// TODO: close the file after we open it
FILE *Fopen(const char * restrict path, const char * restrict mode) {
	FILE *fp = fopen(path, mode);

	if (fp == NULL) {
		fprintf(stderr, "Unable to open file: %s\n", path);
		exit(1);
	}

	return fp;
}

// TODO: could work more on a table implementation that allows for multiple instances
#define TABLE_LENGTH 100

ListNode *hash_table[TABLE_LENGTH] = { NULL };

int str_to_int(char *s) {
	int i = 0;
	while (*s != '\0') {
		i += *s;
		s++;
	}

	return i;
}

// use a string key instead of an int?
int hash(int key) {
	return key % TABLE_LENGTH;
}

typedef struct keyValuePair {
	char *key;
	void *value;
} KeyValuePair;

KeyValuePair *create_key_value_pair(char *key, void *value) {
	KeyValuePair *pair = malloc(sizeof(KeyValuePair));
	pair->key = key;
	pair->value = value;
	return pair;
}

ListNode *create_entry(char *key, void *value) {
	KeyValuePair *key_value_pair = create_key_value_pair(key, value);
	ListNode *entry = create_list_node(key_value_pair);
	return entry;
}

// get the node that contains this key
ListNode *find_node(char *key) {
	int foo = str_to_int(key);
	int hashed_key = hash(foo);

	ListNode *row = hash_table[hashed_key];

	for (ListNode *node = row; node != NULL; node = get_next_node(node)) {
		KeyValuePair *key_value_pair = get_node_payload(node);

		if (strcmp(key_value_pair->key, key) == 0) {
			return node;
		}
	}

	return NULL;
}

void *get(char *key) {
	ListNode *match = find_node(key);

	if (match == NULL) {
		return NULL;
	}
	
	KeyValuePair *key_value_pair = get_node_payload(match);

	return key_value_pair->value;
}

// returns a point to previous payload if there was one
void *set(char *key, void *payload) {
	ListNode *existing_entry = find_node(key);

	if (existing_entry != NULL) {
		KeyValuePair *previous_payload = get_node_payload(existing_entry);
		set_node_payload(existing_entry, payload);
		return previous_payload;
	}

	int foo = str_to_int(key);
	int hashed_key = hash(foo);
	ListNode *new_entry = create_entry(key, payload);
	hash_table[hashed_key] = insert_list_node(hash_table[hashed_key], new_entry);
	return NULL;
}

typedef struct rule {
	int left;
	int right;
} Rule;

void parse_rules(FILE *fp) {
	ssize_t characters_read = 0;

	char *linep = NULL;
	size_t linecapp = 0;

	while ((characters_read = getline(&linep, &linecapp, fp)) > 1) {
		linep[characters_read - 1] = '\0'; // replace the newline with a null terminator
		
		int left;
		int right;

		sscanf(linep, "%d|%d", &left, &right);

		Rule *rule = malloc(sizeof(Rule));
		rule->left = left;
		rule->right = right;

		char *key = malloc(sizeof(char) * characters_read);
		strcpy(key, linep);
		
		set(key, rule);
	}
};

typedef Vector Update;

ListNode *parse_updates(FILE *fp) {
	// keep a list of arrays?
	ListNode *updates = NULL;

	char *linep = NULL;
	size_t linecapp = 0;
	ssize_t characters_read = 0;

	while ((characters_read = getline(&linep, &linecapp, fp)) > 1) {
		Update *update = init_vector();

		linep[characters_read - 1] = '\0'; // replace the newline with a null terminator

		int offset = 0;
		int chars_scanned = 0;
		int scanned_int;

		while (sscanf(linep + offset, "%d%n", &scanned_int, &chars_scanned) > 0) {
			offset += chars_scanned;

			push(update, scanned_int);

			char next_character = *(linep + offset);

			if (next_character == ',') {
				offset += 1;
			}

			if (next_character == '\0') {
				break;
			}
		}


		updates = append_list_node(updates, create_list_node(update));
	}

	return updates;
};

void print_update(Vector *update) {
	int length = get_length(update);

	for (int i = 0; i < length; i++) {
		int page;
		get_element(update, i, &page);
		printf("%d,", page);
	}
	printf("\n");
}

int is_order_okay(int left, int right) {
	char key[6];
	sprintf(key, "%d|%d", right, left);
	// if there is a rule that is the opposite of this arrangement
	if (get(key) == NULL) {
		return 1;
	}

	return 0;
}

int update_breaks_rules(Vector *pages) {
	int length = get_length(pages);

	for (int i = 0; i < length - 1; i++) {
		for (int j = i + 1; j < length; j++) {
			int left;
			int right;
			get_element(pages, i, &left);
			get_element(pages, j, &right);
			if (is_order_okay(left, right) == 0) {
				return 1;
			}
		}
	}

	return 0;
}

// do I need to know the number of pages?
// TODO
void sort_pages(Vector *pages) {
	int length = get_length(pages);

	for (int left_index = 0; left_index < length - 1; left_index++) {
		for (int right_index = left_index + 1; right_index < length; right_index++) {
			int left;
			int right;
			get_element(pages, left_index, &left);
			get_element(pages, right_index, &right);
			// if the order is okay, we do nothing, else we swap them?
			if (is_order_okay(left, right)) {
				continue;
			} else {
				set_element(pages, left_index, right);
				set_element(pages, right_index, left);
			}
		}
	}
}


int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	FILE *fp = Fopen(argv[1], "r");

	parse_rules(fp);
	ListNode *updates = parse_updates(fp);
	
	long running_total = 0;

	for (ListNode *p = updates; p; p = get_next_node(p)) {
		Vector *update = get_node_payload(p);

		// TODO: update this function to accept a vector
		if (update_breaks_rules(update)) {
			printf("Broke rule: ");
			printf("Before: \n");
			print_update(update);

			printf("After: \n");
			sort_pages(update);
			print_update(update);

			int middle_index = get_length(update) / 2;
			int middle_page;
			get_element(update, middle_index, &middle_page);
			printf("Middle index: %d, middle page: %d\n", middle_index, middle_page);
			running_total += middle_page;
		} else {
			printf("Did not break rule: ");
		}
		print_update(update);
	}

	printf("Total: %ld\n", running_total);
	return 0;
}

