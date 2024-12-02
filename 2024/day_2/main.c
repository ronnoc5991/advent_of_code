#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINE 50

typedef struct Node {
	int value;
	struct Node *next;
} Node;

typedef Node * Report;

Node *create_node(int value) {
	Node *node = malloc(sizeof(Node));
	node->value = value;
	node->next = NULL;
	return node;
}

Report parse_report(char *buffer, ssize_t length) {
	Report report = NULL;

	char *head = buffer;
	char *tail = head;

	while (tail < buffer + length) {
		while (isdigit(*tail)) {
			tail++;
		}

		*tail = '\0';
		// parse from head atoi
		int value = atoi(head);
		// put that into a node
		Node *n = create_node(value);

		n->next = report;
		report = n;

		tail++;
		head = tail;
	}

	return report;
}

void free_report(Report report) {
	if (report == NULL) {
		return;
	}

	free_report(report->next);
	free(report);
}

int abs(int value){
	return value < 0 ? value * -1 : value;
}

int is_report_safe(Report report) {
	Node *curr = report;
	Node *next = curr->next;

	int is_safe = 1;

	int is_rising = curr->value < next->value;

	while (next != NULL) {
		int difference = abs(curr->value - next->value);

		if ((curr->value < next->value != is_rising) || difference < 1 || difference > 3) {
			is_safe = 0;
			break;
		}

		curr = next;
		next = next->next;
	}

	return is_safe;
}

int is_report_safe_with_tolerance(Report report) {
	// check if the report is safe as is
	int is_safe_as_is = is_report_safe(report);

	if (is_safe_as_is) {
		return is_safe_as_is;
	}

	// check if the report is safe if we remove the first level
	int is_safe_without_first_level = is_report_safe(report->next);

	if (is_safe_without_first_level) {
		return is_safe_without_first_level;
	}

	// keep track of a left, middle, right
	// point the left at the right, test, then point the left back at the middle
	// increment the pointers...
	Report left = report;
	Report middle = left->next;
	Report right = middle->next;

	while (middle != NULL) {
		// point left at right
		left->next = right;
		// test
		if (is_report_safe(report)) {
			return 1;
		}
		// point left back at middle
		left->next = middle;
		// increment pointers
		left = middle;
		middle = right;

		if (right != NULL) {
			right = right->next;
		}
	}

	return 0;
}

FILE *Fopen(const char * restrict filename, const char * restrict mode) {
	FILE *fp = fopen(filename, mode);

	if (fp == NULL) {
		fprintf(stderr, "Unable to open %s\n", filename);
		exit(1);
	}


	return fp;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file name>\n", argv[0]);
		return 1;
	}

	FILE *fp = Fopen(argv[1], "r");


	size_t maxline = MAXLINE;
	char *buffer = calloc(maxline, sizeof(char));
	ssize_t read;

	int safe_count = 0;

	while ((read = getline(&buffer, &maxline, fp)) != -1) {
		buffer[read - 1] = '\0';

		Report report = parse_report(buffer, read);
		int is_safe_report = is_report_safe_with_tolerance(report);

		if (is_safe_report) {
			safe_count++;
		}

		free_report(report);

		for (int i = 0; i < maxline; i++) {
			buffer[i] = '\0';
		}
	}

	fclose(fp);
	printf("Safe: %d\n", safe_count);

	return 0;
}
