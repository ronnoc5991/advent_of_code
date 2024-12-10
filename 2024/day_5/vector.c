#include "vector.h"
#include <stdlib.h>

#define INCREMENT 10

Vector *init_vector() {
	Vector *vector = malloc(sizeof(Vector));
	set_size(vector, 0);
	set_length(vector, 0);
	set_elements(vector, NULL);
	return vector;
}

// TODO: should probably also signal errors?
void push(Vector *vector, int element) {
	int current_size = get_size(vector);
	int current_length = get_length(vector);

	// out of room!
	if (current_length == current_size / sizeof(int)) {
		// realloc requires bytes... we are keeping the number of ints...
		int new_size = current_size + INCREMENT * sizeof(int);
		int *new_elements = realloc(get_elements(vector), new_size);
		set_elements(vector, new_elements);
		set_size(vector, new_size);
	}

	set_element(vector, current_length, element);
	set_length(vector, current_length + 1);
}

int get_element(Vector *vector, int index, int *out) {
	int current_length = get_length(vector);

	if (index < 0 || index >= current_length) {
		return 1; // unable to perform operation
	}

	int *elements = get_elements(vector);
	*out = *(elements + index);

	return 0;
}

int set_element(Vector *vector, int index, int element) {
	int current_size = get_size(vector);

	if (index < 0 || index >= current_size / sizeof(int)) {
		return 1; // unable to perform operation
	}

	int *elements = get_elements(vector);
	*(elements + index) = element;

	return 0;
}
