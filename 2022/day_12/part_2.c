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

void read_map_into_memory(char map[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], FILE *fp);
void print_map(char map[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS]);
void create_graph(node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], char map[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS]);
node *get_node_from_char(node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], char character_to_find);
node get_null_node();
int get_node_elevation(char character);
int is_position_in_bounds(position *position);
int is_node_already_in_path(node *current_node, node *next_node);
int is_acceptable_elevation_change(node *current_node, node *next_node);
position get_next_position(position *current_position, enum direction direction);
int is_shortest_path_to_next_node(node *current_node, node *next_node);
int get_shortest_path(node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], node *start, node *end);
void reset_graph(node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS]);

int main(void)
{
    char *filename = "input.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    char map[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS];
    read_map_into_memory(map, fp);

    int number_of_starting_points = 0;
    node *starting_points = malloc(sizeof(node));

    node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS];
    create_graph(graph, map);

    for (int i = 0; i < NUMBER_OF_ROWS; i++)
    {
        for (int j = 0; j < NUMBER_OF_COLUMNS; j++)
        {
            node *current_node = &graph[i][j];

            if (current_node->elevation == 'a')
            {
                number_of_starting_points++;
                starting_points = realloc(starting_points, sizeof(node) * number_of_starting_points);
                starting_points[number_of_starting_points - 1] = *current_node;
            }
        }
    }

    node null_node = get_null_node();
    node *end = get_node_from_char(graph, 'E');

    int scores[number_of_starting_points];

    for (int i = 0; i < number_of_starting_points; i++)
    {
        reset_graph(graph);

        node *current_start = &starting_points[i];
        current_start->previous = &null_node;
        current_start->distance = 0;

        int score = get_shortest_path(graph, current_start, end);
        scores[i] = score;
        if (score > 0)
            printf("%i\n", score);
    }

    return 0;
}

void read_map_into_memory(char map[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], FILE *fp)
{
    char current_line[NUMBER_OF_COLUMNS + 2];

    for (int i = 0; i < NUMBER_OF_ROWS; i++)
    {
        fgets(current_line, NUMBER_OF_COLUMNS + 2, fp);

        for (int j = 0; j < NUMBER_OF_COLUMNS; j++)
        {
            map[i][j] = current_line[j];
        }
    }
}

void print_map(char map[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS])
{
    for (int i = 0; i < NUMBER_OF_ROWS; i++)
    {
        for (int j = 0; j < NUMBER_OF_COLUMNS; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}

void create_graph(node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], char map[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS])
{
    for (int i = 0; i < NUMBER_OF_ROWS; i++)
    {
        for (int j = 0; j < NUMBER_OF_COLUMNS; j++)
        {
            node new_node;
            new_node.position.x = j;
            new_node.position.y = i;
            new_node.character = map[i][j];
            new_node.elevation = get_node_elevation(map[i][j]);
            new_node.distance = 100000;
            graph[i][j] = new_node;
        }
    }
}

void reset_graph(node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS])
{
    for (int i = 0; i < NUMBER_OF_ROWS; i++)
    {
        for (int j = 0; j < NUMBER_OF_COLUMNS; j++)
        {
            node *current_node = &graph[i][j];
            current_node->distance = 100000;
            current_node->previous = NULL;
        }
    }
}

int get_shortest_path(node graph[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS], node *start, node *end)
{
    int nodes_in_queue = 1;
    node *queue = malloc(sizeof(node) * nodes_in_queue);

    queue[0] = *start;

    int is_path_found = 0;

    while (!is_path_found)
    {
        if (nodes_in_queue == 0)
        {
            return -1;
        }

        int new_nodes_in_queue = 0;
        node *new_queue = malloc(sizeof(node));

        for (int i = 0; i < nodes_in_queue; i++)
        {
            node *current_node = &queue[i];

            if (current_node->position.x == end->position.x && current_node->position.y == end->position.y)
            {
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

    int step_count = 0;
    node *previous_step = end->previous;

    while (previous_step->position.x != -1 && previous_step->position.y != -1)
    {
        step_count++;
        previous_step = previous_step->previous;
    }

    return step_count;
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