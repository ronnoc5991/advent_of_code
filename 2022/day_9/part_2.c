#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    int x;
    int y;
} position;

#define MAX_INSTRUCTION_LENGTH 10
#define GRID_DIMENSION 700
#define NUMBER_OF_TAILS 9

void clear_grid(int grid[GRID_DIMENSION][GRID_DIMENSION]);
int get_visited_spaces(int grid[GRID_DIMENSION][GRID_DIMENSION]);
void set_position(position *old_position, int new_x, int new_y);
void set_initial_positions(position *head, position tails[NUMBER_OF_TAILS]);
void move_head(position *head, char direction);
void move_tail(position *tail, position *head);
int record_tail_position(position tail, int grid[GRID_DIMENSION][GRID_DIMENSION]);

int main(void)
{
    char *filename = "input.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    int grid[GRID_DIMENSION][GRID_DIMENSION];

    position head;
    position tails[NUMBER_OF_TAILS];

    set_initial_positions(&head, tails);

    clear_grid(grid);

    char current_instruction[MAX_INSTRUCTION_LENGTH];

    while (fgets(current_instruction, MAX_INSTRUCTION_LENGTH, fp) != NULL)
    {
        char *direction = strtok(current_instruction, " ");
        int step_count = atoi(strtok(NULL, " "));

        for (int i = 0; i < step_count; i++)
        {
            move_head(&head, direction[0]);
            // for each tail, update the tail according to the previous node
            for (int j = 0; j < NUMBER_OF_TAILS; j++)
            {
                if (j == 0)
                {
                    move_tail(&tails[j], &head);
                }
                else
                {
                    move_tail(&tails[j], &tails[j - 1]);
                }
            }

            if (!record_tail_position(tails[NUMBER_OF_TAILS - 1], grid))
            {
                return 1;
            }
        }
    }

    int visited_spaces = get_visited_spaces(grid);

    printf("%i\n", visited_spaces);

    return 0;
}

void clear_grid(int grid[GRID_DIMENSION][GRID_DIMENSION])
{
    for (int i = 0; i < GRID_DIMENSION; i++)
    {
        for (int j = 0; j < GRID_DIMENSION; j++)
        {
            grid[i][j] = 0;
        }
    }
}

int get_visited_spaces(int grid[GRID_DIMENSION][GRID_DIMENSION])
{
    int visited_spaces = 0;

    for (int i = 0; i < GRID_DIMENSION; i++)
    {
        for (int j = 0; j < GRID_DIMENSION; j++)
        {
            // printf("%i", grid[i][j]);
            visited_spaces += grid[i][j];
        }
        // printf("\n");
    }

    return visited_spaces;
}

void set_position(position *old_position, int new_x, int new_y)
{
    old_position->x = new_x;
    old_position->y = new_y;
}

void set_initial_positions(position *head, position tails[NUMBER_OF_TAILS])
{
    set_position(head, GRID_DIMENSION / 2, GRID_DIMENSION / 2);

    for (int i = 0; i < NUMBER_OF_TAILS; i++)
    {
        set_position(&tails[i], GRID_DIMENSION / 2, GRID_DIMENSION / 2);
    }
}

void move_head(position *head, char direction)
{
    int new_x = head->x;
    int new_y = head->y;

    switch (direction)
    {
    case 'U':
        new_y -= 1;
        break;
    case 'R':
        new_x += 1;
        break;
    case 'D':
        new_y += 1;
        break;
    case 'L':
        new_x -= 1;
        break;
    }

    printf("head - x: %i y: %i\n", new_x, new_y);
    set_position(head, new_x, new_y);
}

void move_tail(position *tail, position *head)
{
    int new_x = tail->x;
    int new_y = tail->y;

    int x_diff = abs(tail->x - head->x);
    int y_diff = abs(tail->y - head->y);

    if (x_diff <= 1 && y_diff <= 1)
    {
        // they are 'touching', no movement necessary
        return;
    }

    if (x_diff + y_diff == 3)
    {
        new_x += (head->x > tail->x ? 1 : -1);
        new_y += (head->y > tail->y ? 1 : -1);
    }
    else
    {
        new_x += (x_diff == 0 ? 0 : (head->x > tail->x ? 1 : -1));
        new_y += (y_diff == 0 ? 0 : (head->y > tail->y ? 1 : -1));
    }

    set_position(tail, new_x, new_y);
}

int record_tail_position(position tail, int grid[GRID_DIMENSION][GRID_DIMENSION])
{
    if (tail.x < 0 || tail.x > GRID_DIMENSION - 1 || tail.y < 0 || tail.y > GRID_DIMENSION - 1)
    {
        printf("Tail is out of range\n");
        return 0;
    }
    printf("x: %i - y: %i\n", tail.x, tail.y);
    grid[tail.x][tail.y] = 1;
    return 1;
}