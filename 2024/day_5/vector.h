#ifndef VECTOR_H
#define VECTOR_H

#define get_size(vector) (vector->size)
#define set_size(vector, new_size) (vector->size = new_size)

#define get_length(vector) (vector->length)
#define set_length(vector, new_length) (vector->length = new_length)

#define get_elements(vector) (vector->elements)
#define set_elements(vector, new_elements) (vector->elements = new_elements)

// TODO: is there a way to do this in a generic way?
typedef struct vector {
	int size;
	int length;
	int *elements;
} Vector;

Vector *init_vector();

void push(Vector *vector, int element);

int get_element(Vector *vector, int index, int *out);

int set_element(Vector *vector, int index, int element);

#endif
