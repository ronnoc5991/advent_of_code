#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 5
#define NUMBER_OF_SHAPES 3
#define PLAYER_SHAPE_STRING "p"
#define OUTCOME_STRING "o"

int get_score_from_player_shape(char letter);
int get_score_from_outcome(char letter);
int get_index_from_letter(char letter);

//   A B C
// X 3 0 6
// Y 6 3 0
// Z 0 6 3

// outcome_score_matrix[opponent_shape][player_shape] = outcome_score
int outcome_score_matrix[NUMBER_OF_SHAPES][NUMBER_OF_SHAPES] = {{3, 6, 0}, {0, 3, 6}, {6, 0, 3}};

//   A B C
// X 3 1 2
// Y 1 2 3
// Z 2 3 1

// player_shape_score_matrix[opponent_shape][outcome] = player_shape_score
int player_shape_score_matrix[NUMBER_OF_SHAPES][NUMBER_OF_SHAPES] = {{3, 1, 2}, {1, 2, 3}, {2, 3, 1}};

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: ./rps filename second_column_meaning\n");
        return 1;
    }

    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s\n", filename);
        return 1;
    }

    char *second_column_meaning = argv[2];

    if (strcmp(second_column_meaning, PLAYER_SHAPE_STRING) != 0 && strcmp(second_column_meaning, OUTCOME_STRING) != 0)
    {
        printf("Invalid second_column_meaning passed.  Valid options: '%s' or '%s'\n", PLAYER_SHAPE_STRING, OUTCOME_STRING);
        return 1;
    }

    int total_score = 0;
    char current_line[MAX_LINE_LENGTH];
    int is_second_column_player_shape = strcmp(second_column_meaning, PLAYER_SHAPE_STRING) == 0;

    while (fgets(current_line, MAX_LINE_LENGTH, fp))
    {
        if (is_second_column_player_shape)
        {
            total_score += get_score_from_player_shape(current_line[2]);
            total_score += outcome_score_matrix[get_index_from_letter(current_line[0])][get_index_from_letter(current_line[2])];
        }
        else
        {
            total_score += get_score_from_outcome(current_line[2]);
            total_score += player_shape_score_matrix[get_index_from_letter(current_line[0])][get_index_from_letter(current_line[2])];
        }
    }

    fclose(fp);

    printf("%i", total_score);
}

int get_score_from_player_shape(char letter)
{
    if (letter == 'X')
    {
        return 1;
    }
    else if (letter == 'Y')
    {
        return 2;
    }
    else if (letter == 'Z')
    {
        return 3;
    }
    printf("Unexpected player shape received: %c\n", letter);
    return 0;
}

int get_score_from_outcome(char letter)
{
    if (letter == 'X')
    {
        return 0;
    }
    else if (letter == 'Y')
    {
        return 3;
    }
    else if (letter == 'Z')
    {
        return 6;
    }
    printf("Unexpected outcome received: %c\n", letter);
    return 0;
}

int get_index_from_letter(char letter)
{
    switch (letter)
    {
    case 'A':
    case 'X':
        return 0;
    case 'B':
    case 'Y':
        return 1;
    case 'C':
    case 'Z':
        return 2;
    }
    return 0;
}