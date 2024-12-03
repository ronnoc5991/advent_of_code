#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DO_MODE 1
#define DONT_MODE 0

FILE *Fopen(const char * restrict path, const char * restrict mode) {
	FILE *fp = fopen(path, mode);

	if (fp == NULL) {
		fprintf(stderr, "Unable to open file: %s\n", path);
		exit(1);
	}

	return fp;
}

int match(FILE *fp, char expected) {
	char found = fgetc(fp);
	if (found != expected) {
		ungetc(found, fp);
		return 0;
	}

	return 1;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <path to input>\n", argv[0]);
		exit(1);
	}

	FILE *fp = Fopen(argv[1], "r");

	long int total = 0;

	int mode = DO_MODE;

	for (int c; (c = fgetc(fp)) != EOF;) {
		if (mode == DO_MODE && c == 'm') {
			int first;
			int second;

			if(fscanf(fp, "ul(%d,%d", &first, &second) == 2) {
				if (fgetc(fp) != ')') {
					continue;
				}
				total = total + (first * second);
			}
		} else if (mode == DO_MODE && c == 'd') {
			if (
					match(fp, 'o') &&
					match(fp, 'n') &&
					match(fp, '\'') &&
					match(fp, 't') &&
					match(fp, '(') &&
					match(fp, ')')
			   ) {
				mode = DONT_MODE;
			}
		} else if (mode == DONT_MODE && c == 'd') {
			if (
					match(fp, 'o') &&
					match(fp, '(') &&
					match(fp, ')')
			   ) {
				mode = DO_MODE;
			}
		}
	}

	printf("Total: %lu\n", total);
}
