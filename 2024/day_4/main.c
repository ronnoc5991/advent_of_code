#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

FILE *Fopen(const char * restrict path, const char * restrict mode) {
	FILE *fp = fopen(path, mode);

	if (fp == NULL) {
		fprintf(stderr, "Unable to open file: %s\n", path);
		exit(1);
	}

	return fp;
}

int is_within_bounds(int n, int min, int max) {
	return min <= n && n <= max;
}

enum Direction {
	ALL,
	UPLEFT,
	UPMIDDLE,
	UPRIGHT,
	MIDDLELEFT,
	MIDDLERIGHT,
	DOWNLEFT,
	DOWNMIDDLE,
	DOWNRIGHT
};

int search_adjacent_cells(int row, int column, char *string, int rows, int columns, char grid[rows][columns], enum Direction direction) {
	if (!is_within_bounds(row, 0, rows - 1) || !is_within_bounds(column, 0, columns - 1)) {
		return 0;
	}

	if (grid[row][column] != *string) {
		return 0;
	}

	if (*(string + 1) == '\0') {
		return 1;
	}

	int count = 0;
	// compute the positions of the adjacent cells

	// check upper left
	if (direction == ALL || direction == UPLEFT) {
		count += search_adjacent_cells(row - 1, column - 1, string + 1, rows, columns, grid, UPLEFT);
	}
	// check upper middle
	if (direction == ALL || direction == UPMIDDLE) {
		count += search_adjacent_cells(row - 1, column, string + 1, rows, columns, grid, UPMIDDLE);
	}
	// check upper right
	if (direction == ALL || direction == UPRIGHT) {
		count += search_adjacent_cells(row - 1, column + 1, string + 1, rows, columns, grid, UPRIGHT);
	}
	// check middle left
	if (direction == ALL || direction == MIDDLELEFT) {
		count += search_adjacent_cells(row, column - 1, string + 1, rows, columns, grid, MIDDLELEFT);
	}
	// check middle right
	if (direction == ALL || direction == MIDDLERIGHT) {
		count += search_adjacent_cells(row, column + 1, string + 1, rows, columns, grid, MIDDLERIGHT); 
	}
	// check lower left
	if (direction == ALL || direction == DOWNLEFT) {
		count += search_adjacent_cells(row + 1, column - 1, string + 1, rows, columns, grid, DOWNLEFT);
	}
	// check lower middle
	if (direction == ALL || direction == DOWNMIDDLE) {
		count += search_adjacent_cells(row + 1, column, string + 1, rows, columns, grid, DOWNMIDDLE);
	}
	// check lower right
	if (direction == ALL || direction == DOWNRIGHT) {
		count += search_adjacent_cells(row + 1, column + 1, string + 1, rows, columns, grid, DOWNRIGHT);
	}

	return count;
}

// take the grid and dimensions?
int count_xmas(int rows, int columns, char grid[rows][columns]) {
	int count = 0;
	int xsfound = 0;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			char c = grid[i][j];
			if (c == 'X') {
				xsfound++;
				count += search_adjacent_cells(i, j, "XMAS", rows, columns, grid, ALL);
			}
		}
	}

	printf("xs: %d\n", xsfound);

	return count;
}

int part_one_main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	FILE *fp = Fopen(argv[1], "r");

	char *linep = NULL;
	size_t linecapp = 0;
	size_t columns = 0;
	size_t rows = 0;
	ssize_t chars_read = 0;


	while ((chars_read = getline(&linep, &linecapp, fp)) != EOF) {
		if (columns == 0) {
			linep[chars_read - 1] = '\0';
			columns = strlen(linep);
		}

		rows++;	
	}

	free(linep);

	printf("%zu, %zu\n", columns, rows);

	// reset the position in the stream
	fseek(fp, 0, SEEK_SET);

	// could also calloc this?
	char grid[rows][columns];

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			char c;
			while (!isalpha(c = fgetc(fp))) {}

			grid[i][j] = c;
		}
	}

	int instances = count_xmas(rows, columns, grid);
	printf("Instances: %d\n", instances);
	return 0;
}

int is_xmas(int row, int column, int rows, int columns, char grid[rows][columns]) {
	// grab opposing pairs
	int row_above = row - 1;
	int row_below = row + 1;
	int column_left = column - 1;
	int column_right = column + 1;

	if (
			!is_within_bounds(row_above, 0, rows - 1) ||
			!is_within_bounds(row_below, 0, rows - 1) ||
			!is_within_bounds(column_left, 0, columns - 1) ||
			!is_within_bounds(column_right, 0, columns - 1)
	   ) {
		return 0;
	}

	char upper_left = grid[row_above][column_left];
	char upper_right = grid[row_above][column_right];
	char lower_left = grid[row_below][column_left];
	char lower_right = grid[row_below][column_right];

	if (
			((upper_left == 'M' && lower_right == 'S') ||
			 (upper_left == 'S' && lower_right == 'M')) &&
			((upper_right == 'M' && lower_left == 'S') ||
			 (upper_right == 'S' && lower_left == 'M'))
	   ) {
		return 1;
	}

	return 0;
}

int count_xmas_two(int rows, int columns, char grid[rows][columns]) {
	int count = 0;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			char c = grid[i][j];
			if (c == 'A') {
				count += is_xmas(i, j, rows, columns, grid);
			}
		}
	}

	return count;
}

// part two main
int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	FILE *fp = Fopen(argv[1], "r");

	char *linep = NULL;
	size_t linecapp = 0;
	size_t columns = 0;
	size_t rows = 0;
	ssize_t chars_read = 0;

	while ((chars_read = getline(&linep, &linecapp, fp)) != EOF) {
		if (columns == 0) {
			linep[chars_read - 1] = '\0';
			columns = strlen(linep);
		}

		rows++;	
	}

	free(linep);

	printf("%zu, %zu\n", columns, rows);

	// reset the position in the stream
	fseek(fp, 0, SEEK_SET);

	// could also calloc this?
	char grid[rows][columns];

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			char c;
			while (!isalpha(c = fgetc(fp))) {}

			grid[i][j] = c;
		}
	}

	int instances = count_xmas_two(rows, columns, grid);
	printf("Instances: %d\n", instances);
}
