#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define FORREST_WIDTH 99
#define FORREST_HEIGHT 99

void save_row(char row[FORREST_WIDTH], int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index);
int get_tree_score(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index);
int get_top_score(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index);
int get_right_score(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index);
int get_bottom_score(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index);
int get_left_score(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index);

int main(void)
{
    char *filename = "input.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    int forrest[FORREST_HEIGHT][FORREST_WIDTH];

    char current_row[FORREST_WIDTH];
    int row_index = 0;

    while (fgets(current_row, FORREST_WIDTH + 2, fp) != NULL)
    {
        save_row(current_row, forrest, row_index);
        row_index++;
    }

    int highest_tree_score = 0;

    for (int row_index = 0; row_index < FORREST_HEIGHT; row_index++)
    {
        for (int column_index = 0; column_index < FORREST_WIDTH; column_index++)
        {
            int tree_score = get_tree_score(forrest, row_index, column_index);

            if (tree_score > highest_tree_score)
            {
                highest_tree_score = tree_score;
            }
        }
    }

    printf("%i", highest_tree_score);

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

int get_tree_score(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index)
{
    int scores[4] = {get_top_score(forrest, row_index, column_index), get_right_score(forrest, row_index, column_index), get_bottom_score(forrest, row_index, column_index), get_left_score(forrest, row_index, column_index)};

    int product = 1;

    for (int i = 0; i < 4; i++)
    {
        product *= scores[i];
    }

    return product;
}

int get_top_score(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index)
{
    int tree_height = forrest[row_index][column_index];
    int top_score = 0;

    for (int i = row_index - 1; i >= 0; i--)
    {
        top_score++;
        int comparison_tree_height = forrest[i][column_index];

        if (comparison_tree_height >= tree_height)
        {
            break;
        }
    }

    return top_score;
}

int get_right_score(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index)
{
    int tree_height = forrest[row_index][column_index];
    int right_score = 0;

    for (int i = column_index + 1; i < FORREST_WIDTH; i++)
    {
        right_score++;
        int comparison_tree_height = forrest[row_index][i];

        if (comparison_tree_height >= tree_height)
        {
            break;
        }
    }

    return right_score;
}

int get_bottom_score(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index)
{
    int tree_height = forrest[row_index][column_index];
    int bottom_score = 0;

    for (int i = row_index + 1; i < FORREST_HEIGHT; i++)
    {
        bottom_score++;
        int comparison_tree_height = forrest[i][column_index];

        if (comparison_tree_height >= tree_height)
        {
            break;
        }
    }

    return bottom_score;
}

int get_left_score(int forrest[FORREST_WIDTH][FORREST_HEIGHT], int row_index, int column_index)
{
    int tree_height = forrest[row_index][column_index];
    int left_score = 0;

    for (int i = column_index - 1; i >= 0; i--)
    {
        left_score++;
        int comparison_tree_height = forrest[row_index][i];

        if (comparison_tree_height >= tree_height)
        {
            break;
        }
    }

    return left_score;
}