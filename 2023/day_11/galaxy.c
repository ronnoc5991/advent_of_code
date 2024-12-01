#define NUMBER_OF_GALAXIES 455 // this feels like cheating
#define COORDINATE_COLUMN_INDEX 0
#define COORDINATE_ROW_INDEX 1

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