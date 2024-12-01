#include <stdio.h>
#include <stdlib.h>

#define LINE 142
#define COLUMNS 140
#define ROWS 140

// TODO: Think about moving galaxy stuff to another file? abstract it

#define NUMBER_OF_GALAXIES 455 // this feels like cheating
#define COORDINATE_COLUMN_INDEX 0
#define COORDINATE_ROW_INDEX 1
#define CONTAINS_GALAXY 1
#define EXPANSION_COEFFICIENT 999999

int galaxies[NUMBER_OF_GALAXIES][2]; // 455 rows and 2 columns?

void set_galaxy_coordinates(int *coordinates, int column, int row)
{
    coordinates[COORDINATE_COLUMN_INDEX] = column;
    coordinates[COORDINATE_ROW_INDEX] = row;
}

int get_galaxy_column(int *coordinates)
{
    return coordinates[COORDINATE_COLUMN_INDEX];
}

int get_galaxy_row(int *coordinates)
{
    return coordinates[COORDINATE_ROW_INDEX];
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Expected a single argument: filename\n");
        return 1;
    }

    char *filename = argv[1];
    FILE *fp;

    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Unable to open file '%s'\n", filename);
        return 1;
    }

    int columns[COLUMNS]; // on the initial pass, flip each spot to 1 if we found a galaxy here
    int rows[ROWS]; // then create the arrays that contain the row/column numbers to expand based on whether or not they have a 1 in these arrays

    for (int i = 0; i < ROWS; i++) {
        columns[i] = 0;
        rows[i] = 0;
    }

    char line[LINE];

    int row, column, gp, c;

    gp = 0;

    for (row = 0; fgets(line, LINE, fp); row++) {
        for (column = 0;  (c = line[column] != '\n') && c != EOF && column < COLUMNS; column++) {
            if (line[column] == '#') {
                set_galaxy_coordinates(galaxies[gp++], column, row);
                rows[row] = CONTAINS_GALAXY;
                columns[column] = CONTAINS_GALAXY;
            }
        }
    }

    int rows_to_expand[10];
    int columns_to_expand[10];

    for (int i = 0; i < 10; i++) {
        rows_to_expand[i] = -1;
        columns_to_expand[i] = -1;
    }

    for (int i = 0, rp = 0, cp = 0; i < ROWS; i++) {
        if (rows[i] != CONTAINS_GALAXY)
            rows_to_expand[rp++] = i;
        if (columns[i] != CONTAINS_GALAXY)
            columns_to_expand[cp++] = i;
    }

    // expand the universe
    for (int i = 0; i < NUMBER_OF_GALAXIES; i++) {
        int galaxy_row = get_galaxy_row(galaxies[i]);
        int galaxy_column = get_galaxy_column(galaxies[i]);

        int row = galaxy_row;
        int column = galaxy_column;

        for (int j = 0; j < 10; j++) {
            if (rows_to_expand[j] >= 0 && rows_to_expand[j] < galaxy_row)
                row+= EXPANSION_COEFFICIENT;
            if (columns_to_expand[j] >= 0 && columns_to_expand[j] < galaxy_column)
                column+= EXPANSION_COEFFICIENT;
        }

        printf("new coords: %d, %d\n", column, row);

        set_galaxy_coordinates(galaxies[i], column, row);
    }

    unsigned long sum = 0;

    for (int i = 0; i < NUMBER_OF_GALAXIES; i++) {
        for (int j = i + 1; j < NUMBER_OF_GALAXIES; j++) {
            int vertical_distance = abs(get_galaxy_row(galaxies[i]) - get_galaxy_row(galaxies[j]));
            int horizontal_distance = abs(get_galaxy_column(galaxies[i]) - get_galaxy_column(galaxies[j]));
            sum += (vertical_distance + horizontal_distance);
        }
    }

    printf("Sum: %lu\n", sum);

    return 0;
}

// 396735318
// 447073334102