#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define FORREST_WIDTH 99
#define FORREST_HEIGHT 99

void save_row(char row[FORREST_WIDTH], int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index);
int is_tree_visible_from_outside(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index);
int is_tree_visible_from_top(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index);
int is_tree_visible_from_right(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index);
int is_tree_visible_from_bottom(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index);
int is_tree_visible_from_left(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index);

int main(void)
{
    char *filename = "input.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    int forrest[FORREST_WIDTH][FORREST_HEIGHT];

    char current_row[FORREST_WIDTH];
    int row_index = 0;

    while (fgets(current_row, FORREST_WIDTH + 2, fp) != NULL)
    {
        save_row(current_row, forrest, row_index);
        row_index++;
    }

    int visible_tree_count = 0;

    // go over every tree
    // check if it is visible from any side
    // if yes, add it to the count

    for (int row_index = 0; row_index < FORREST_WIDTH; row_index++)
    {
        // for every row
        for (int column_index = 0; column_index < FORREST_HEIGHT; column_index++)
        {
            // for every column in every row
            if (is_tree_visible_from_outside(forrest, row_index, column_index))
            {
                visible_tree_count++;
                printf("%i", forrest[row_index][column_index]);
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("%i", visible_tree_count);

    return 0;
}

void save_row(char row[FORREST_WIDTH], int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index)
{
    for (int i = 0; i < FORREST_WIDTH; i++)
    {
        if (isnumber(row[i]))
        {
            forrest[row_index][i] = row[i] - '0';
        }
    }
}

int is_tree_visible_from_outside(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index)
{
    if (is_tree_visible_from_top(forrest, row_index, column_index) || is_tree_visible_from_right(forrest, row_index, column_index) || is_tree_visible_from_bottom(forrest, row_index, column_index) || is_tree_visible_from_left(forrest, row_index, column_index))
    {
        return 1;
    }

    return 0;
}

int is_tree_visible_from_top(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index)
{
    int tree_height = forrest[row_index][column_index];

    for (int i = 0; i < row_index; i++)
    {
        int comparison_tree_height = forrest[i][column_index];

        if (comparison_tree_height >= tree_height)
        {
            return 0;
        }
    }

    return 1;
}

int is_tree_visible_from_right(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index)
{
    int tree_height = forrest[row_index][column_index];

    for (int i = FORREST_WIDTH - 1; i > column_index; i--)
    {
        int comparison_tree_height = forrest[row_index][i];

        if (comparison_tree_height >= tree_height)
        {
            return 0;
        }
    }
    return 1;
}

int is_tree_visible_from_bottom(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index)
{
    int tree_height = forrest[row_index][column_index];

    for (int i = FORREST_HEIGHT - 1; i > row_index; i--)
    {
        int comparison_tree_height = forrest[i][column_index];

        if (comparison_tree_height >= tree_height)
        {
            return 0;
        }
    }

    return 1;
}

int is_tree_visible_from_left(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index)
{
    int tree_height = forrest[row_index][column_index];

    for (int i = 0; i < column_index; i++)
    {
        int comparison_tree_height = forrest[row_index][i];

        if (comparison_tree_height >= tree_height)
        {
            return 0;
        }
    }
    return 1;
}
