#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int x;
    int y;
} position;

struct node
{
    position position;
    int elevation;
    char character;
    int distance;
    struct node *previous;
};

typedef struct node node;

#define NUMBER_OF_DIRECTIONS 4
#define NUMBER_OF_ROWS 41
#define NUMBER_OF_COLUMNS 113
#define STARTING_CHAR 'S'
#define ENDING_CHAR 'E'

enum direction
{
    Up,
    Right,
    Down,
    Left
};

void create_graph(node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], FILE *fp);
node *get_node_from_char(node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], char character_to_find);
node get_null_node();
int get_node_elevation(char character);
int is_position_in_bounds(position *position);
int is_node_already_in_path(node *current_node, node *next_node);
int is_acceptable_elevation_change(node *current_node, node *next_node);
position get_next_position(position *current_position, enum direction direction);
int is_shortest_path_to_next_node(node *current_node, node *next_node);

int main(void)
{
    char *filename = "test.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS];
    create_graph(graph, fp);

    node null_node = get_null_node();
    node *start = get_node_from_char(graph, 'S');
    node *end = get_node_from_char(graph, 'E');
    start->previous = &null_node;
    start->distance = 0;

    int nodes_in_queue = 1;                              // I will want to reset this every loop
    node *queue = malloc(sizeof(node) * nodes_in_queue); // I will want to realloc this every loop

    queue[0] = *start;

    int is_path_found = 0;

    while (!is_path_found)
    {
        int new_nodes_in_queue = 0;
        node *new_queue = malloc(sizeof(node));

        for (int i = 0; i < nodes_in_queue; i++)
        {
            node *current_node = &queue[i];

            if (current_node->position.x == end->position.x && current_node->position.y == end->position.y)
            {
                printf("Reached the end!\n");
                is_path_found = 1;
                break;
            }

            for (int j = 0; j < NUMBER_OF_DIRECTIONS; j++)
            {
                position next_position = get_next_position(&current_node->position, j);

                // if next position is out of bounds, abandon it
                if (!is_position_in_bounds(&next_position))
                {
                    continue;
                }

                node *next_node = &graph[next_position.y][next_position.x];

                // if next node is already in path, abandon it
                if (is_node_already_in_path(current_node, next_node))
                {
                    continue;
                }

                // if elevation change is unacceptable, abandon it
                if (!is_acceptable_elevation_change(current_node, next_node))
                {
                    continue;
                }

                // if it is not the shortest path to the next node, we abandon it
                if (!is_shortest_path_to_next_node(current_node, next_node))
                {
                    continue;
                }

                printf("x: %i, y: %i\n", next_node->position.x, next_node->position.y);

                next_node->distance = current_node->distance + 1;
                next_node->previous = current_node;
                // increase new node count
                // reallocate memory for the new size
                // add the next node at the new node count -1 index
                new_nodes_in_queue++;
                new_queue = realloc(new_queue, sizeof(node) * new_nodes_in_queue);
                new_queue[new_nodes_in_queue - 1] = *next_node;
            }
        }

        queue = new_queue;
        nodes_in_queue = new_nodes_in_queue;
    }

    // walk backwards from the end to count the steps to the beginning
    int step_count = 0;
    node *previous_step = end;
    do
    {
        previous_step = previous_step->previous;
        step_count++;
    } while (previous_step->position.x != -1 && previous_step->position.y != -1);

    printf("Steps: %i\n", step_count);
    return 0;
}

int is_shortest_path_to_next_node(node *current_node, node *next_node)
{
    return current_node->distance + 1 < next_node->distance;
}

int is_position_in_bounds(position *position)
{
    if (position->x < 0 || position->x >= NUMBER_OF_COLUMNS || position->y < 0 || position->y >= NUMBER_OF_ROWS)
        return 0;

    return 1;
}

position get_next_position(position *current_position, enum direction direction)
{
    position next_position;
    next_position.x = current_position->x;
    next_position.y = current_position->y;

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
    default:
        break;
    }

    return next_position;
}

void create_graph(node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], FILE *fp)
{
    char current_line[NUMBER_OF_COLUMNS + 2];

    for (int i = 0; i < NUMBER_OF_ROWS; i++)
    {
        fgets(current_line, NUMBER_OF_COLUMNS + 2, fp);

        for (int j = 0; j < NUMBER_OF_COLUMNS; j++)
        {
            node new_node;
            new_node.position.x = j;
            new_node.position.y = i;
            new_node.character = current_line[j];
            new_node.elevation = get_node_elevation(current_line[j]);
            new_node.distance = 100000;
            graph[i][j] = new_node;
        }
    }
}

int get_node_elevation(char character)
{
    if (character == 'S')
    {
        return 'a';
    }

    if (character == 'E')
    {
        return 'z';
    }

    return character;
}

node *get_node_from_char(node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], char character_to_find)
{
    for (int i = 0; i < NUMBER_OF_ROWS; i++)
    {
        for (int j = 0; j < NUMBER_OF_COLUMNS; j++)
        {
            if (graph[i][j].character == character_to_find)
            {
                return &graph[i][j];
                break;
            }
        }
    }
    return &graph[0][0];
}

node get_null_node()
{
    node null_node;
    null_node.position.x = -1;
    null_node.position.y = -1;
    return null_node;
}

int is_acceptable_elevation_change(node *current_node, node *next_node)
{
    return current_node->elevation - next_node->elevation >= -1;
}

int is_node_already_in_path(node *current_node, node *next_node)
{
    node *previous_node = current_node->previous;

    while (previous_node->position.x != -1 && previous_node->position.y != -1)
    {
        if (previous_node->position.x == next_node->position.x && previous_node->position.y == next_node->position.y)
        {
            return 1;
        }

        previous_node = previous_node->previous;
    }

    return 0;
}