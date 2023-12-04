#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 300
#define MAX_TOKEN_SIZE 8
#define MIN_X 479
// #define MIN_X 494
#define MAX_X 548
// #define MAX_X 503
#define MAX_Y 174
// #define MAX_Y 9
#define MAP_WIDTH MAX_X - MIN_X + 1
#define MAP_HEIGHT MAX_Y + 1
#define FLOOR_Y MAX_Y + 2
#define SAND_SOURCE_X 500 - MIN_X
#define SAND_SOURCE_Y 0
#define ROCK '#'
#define AIR '.'
#define SAND_SOURCE '+'
#define SAND 'o'

enum direction
{
    Up,
    Right,
    Down,
    Left
};

typedef struct
{
    int x;
    int y;
} position;

void initialize_map(char map[MAP_HEIGHT][MAP_WIDTH], FILE *fp);
void print_map(char map[MAP_HEIGHT][MAP_WIDTH]);
void clean_up_token(char *token);
position get_position_from_token(char *token);
position get_next_position(position current_position, enum direction direction);
void place_rocks(char map[MAP_HEIGHT][MAP_WIDTH], position from, position to);
position get_sand_resting_position(char map[MAP_HEIGHT][MAP_WIDTH], int x, int y);
int is_valid_move(char map[MAP_HEIGHT][MAP_WIDTH], position position);
int is_position_in_bounds(position current_position);
int does_sand_rest_in_bounds(char map[MAP_HEIGHT][MAP_WIDTH], int x, int y);

int main(void)
{
    char *filename = "input.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    char map[MAP_HEIGHT][MAP_WIDTH];
    initialize_map(map, fp);

    int grain_count = 0;

    while (does_sand_rest_in_bounds(map, SAND_SOURCE_X, SAND_SOURCE_Y))
    {
        grain_count++;
    }

    print_map(map);
    printf("Grain Count: %i\n", grain_count);

    return 0;
}

void initialize_map(char map[MAP_HEIGHT][MAP_WIDTH], FILE *fp)
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            map[i][j] = AIR;
        }
    }

    char current_line[MAX_LINE_LENGTH];
    char delimiter[] = " -> ";

    map[SAND_SOURCE_Y][SAND_SOURCE_X] = SAND_SOURCE;

    while (fgets(current_line, MAX_LINE_LENGTH, fp) != NULL)
    {
        int token_count = 0;
        char *token = strtok(current_line, delimiter);
        char **tokens = malloc(sizeof(sizeof(char) * MAX_TOKEN_SIZE));

        while (token != NULL)
        {
            token_count++;
            clean_up_token(token);
            tokens = realloc(tokens, sizeof(char) * MAX_TOKEN_SIZE * token_count);
            tokens[token_count - 1] = token;
            token = strtok(NULL, delimiter);
        }

        position positions[token_count];

        for (int i = 0; i < token_count; i++)
        {
            position new_position = get_position_from_token(tokens[i]);
            positions[i] = new_position;
        }

        for (int i = 0; i < token_count - 1; i++)
        {
            place_rocks(map, positions[i], positions[i + 1]);
        }
    }
}

void print_map(char map[MAP_HEIGHT][MAP_WIDTH])
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

void clean_up_token(char *token)
{
    if (token[strlen(token) - 1] == '\n')
        token[strlen(token) - 1] = 0;
}

position get_position_from_token(char *token)
{
    position new_position;
    char *delimiter = ",";
    new_position.x = atoi(strtok(token, delimiter)) - MIN_X;
    new_position.y = atoi(strtok(NULL, delimiter));
    return new_position;
}

void place_rocks(char map[MAP_HEIGHT][MAP_WIDTH], position from, position to)
{
    int is_up = from.y > to.y;
    int is_right = from.x < to.x;
    int is_down = from.y < to.y;
    int is_left = from.x > to.x;

    map[from.y][from.x] = ROCK;
    map[to.y][to.x] = ROCK;

    enum direction direction;

    if (is_up)
        direction = Up;
    else if (is_right)
        direction = Right;
    else if (is_down)
        direction = Down;
    else if (is_left)
        direction = Left;

    position next_position = get_next_position(from, direction);

    while (next_position.x != to.x || next_position.y != to.y)
    {
        map[next_position.y][next_position.x] = ROCK;
        next_position = get_next_position(next_position, direction);
    }
}

position get_next_position(position current_position, enum direction direction)
{
    position next_position;
    next_position.x = current_position.x;
    next_position.y = current_position.y;

    switch (direction)
    {
    case Up:
        next_position.y--;
        break;
    case Right:
        next_position.x++;
        break;
    case Down:
        next_position.y++;
        break;
    case Left:
        next_position.x--;
        break;
    }

    return next_position;
}

int does_sand_rest_in_bounds(char map[MAP_HEIGHT][MAP_WIDTH], int x, int y)
{
    position current_position;
    current_position.x = x;
    current_position.y = y;

    int can_move = 1;

    while (can_move)
    {
        // need to check here if the sand is out of bounds?
        if (!is_position_in_bounds(current_position))
        {
            return 0;
        }

        position position_down = get_next_position(current_position, Down);
        position position_down_left = get_next_position(position_down, Left);
        position position_down_right = get_next_position(position_down, Right);

        if (is_valid_move(map, position_down))
        {
            current_position = position_down;
        }
        else if (is_valid_move(map, position_down_left))
        {
            current_position = position_down_left;
        }
        else if (is_valid_move(map, position_down_right))
        {
            current_position = position_down_right;
        }
        else
        {
            can_move = 0;
        }
    }

    map[current_position.y][current_position.x] = SAND;
    return 1;
}

int is_valid_move(char map[MAP_HEIGHT][MAP_WIDTH], position position)
{
    return map[position.y][position.x] != SAND && map[position.y][position.x] != ROCK;
}

int is_position_in_bounds(position current_position)
{
    return current_position.y < MAX_Y + 1;
}