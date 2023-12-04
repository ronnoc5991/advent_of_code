#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 80
#define STARTING_VALVE_NAME "AA"

typedef struct
{
    char *name;
    int flow_rate;
    int is_open;
    char **neighbors;
} valve;

// we ARE allowed to revisit nodes
// need to find a path through the nodes that maximizes the score in the given time

void parse_valves(FILE *fp, valve **valves, int *number_of_valves);
char *get_valve_name(char *line);
int get_valve_flow_rate(char *line);
char **get_valve_neighbors(char *line);
valve *get_starting_valve(valve **valves, int number_of_valves);
int are_there_open_valves(valve *valves, int number_of_valves);

int main(int argc, char **argv)
{
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file '%s'\n", filename);
        return 1;
    }

    int number_of_valves = 0;
    valve *valves = malloc(sizeof(valve) * number_of_valves);

    parse_valves(fp, &valves, &number_of_valves);

    int score = 0;
    int minutes_left = 30; // decrement this as necessary
    valve *current_valve = get_starting_valve(&valves, number_of_valves);

    printf("%s\n", current_valve->name);

    while (are_there_open_valves(valves, number_of_valves))
    {
        int max_score = 0;
        char *next_valve_name;

        // create array of open valves?
        // for each open valve, calculate its score
        // for all open valves, calculate how long it would take to travel there AND open the valve
        // score = flow_rate * minutes left
        // if score > max_score
        // replace the max_score
        // save this valve's name for later

        // after finding the next best valve
        // update the starting valve
        // add the max score to the score
        // repeat
    }

    return 0;
}

void parse_valves(FILE *fp, valve **valves, int *number_of_valves)
{
    char current_line[MAX_LINE_LENGTH];

    while (fgets(current_line, MAX_LINE_LENGTH, fp) != NULL)
    {
        int length = strlen(current_line);
        if (current_line[length - 1] == '\n')
            current_line[length - 1] = 0;

        valve new_valve;
        new_valve.name = get_valve_name(current_line);
        new_valve.flow_rate = get_valve_flow_rate(current_line);
        new_valve.is_open = 0;
        new_valve.neighbors = get_valve_neighbors(current_line);

        (*number_of_valves)++;
        *valves = realloc(*valves, sizeof(valve) * *number_of_valves);
        (*valves)[*number_of_valves - 1] = new_valve;
    }
}

char *get_valve_name(char *line)
{
    char *valve_name = malloc(sizeof(char) * 3);
    valve_name[0] = line[6];
    valve_name[1] = line[7];
    valve_name[2] = '\0';
    return valve_name;
}

int get_valve_flow_rate(char *line)
{
    int char_count = 1;
    char *valve_flow_rate = malloc(sizeof(char) * char_count);
    char *start = strstr(line, "=") + 1;
    char *end = strstr(line, ";");

    while (start != end)
    {
        char_count++;
        valve_flow_rate = realloc(valve_flow_rate, sizeof(*valve_flow_rate) * char_count);
        valve_flow_rate[char_count - 2] = *start;
        start++;
    }

    valve_flow_rate[char_count - 1] = '\0';

    return atoi(valve_flow_rate);
}

char **get_valve_neighbors(char *line)
{
    int neighbor_count = 0;
    char **valve_neighbors = malloc(sizeof(char *) * neighbor_count);

    int has_more_neighbors = 1;
    char *ptr = &line[strlen(line)] - 2;

    while (has_more_neighbors)
    {
        char *current_neighbor = malloc(sizeof(char) * 3);
        current_neighbor[0] = *ptr;
        current_neighbor[1] = *(ptr + 1);
        current_neighbor[2] = '\0';

        neighbor_count++;
        valve_neighbors = realloc(valve_neighbors, sizeof(char *) * neighbor_count);
        valve_neighbors[neighbor_count - 1] = current_neighbor;

        if (*(ptr - 2) != ',')
            has_more_neighbors = 0;
        else
            ptr -= 4;
    }

    return valve_neighbors;
}

int are_there_open_valves(valve *valves, int number_of_valves)
{
    for (int i = 0; i < number_of_valves; i++)
    {
        if (valves[i].is_open)
            return 1;
    }

    return 0;
}

// could change this to get valve by name, then it would be reusable
valve *get_starting_valve(valve **valves, int number_of_valves)
{
    for (int i = 0; i < number_of_valves; i++)
    {
        valve *current_valve = &(*valves)[i];

        if (strcmp(current_valve->name, STARTING_VALVE_NAME) == 0)
            return current_valve;
    }

    return &(*valves)[1];
}

// if the valves had pointers to each other, we could forget about looking them up in the array everytime we want to move
// create a function that takes the valves, the starting valve and the ending valve
// calculates the distance to each node
// from start
// calculate the shortest distance to each node
// then calculate the scores for each node based on the time to get there?
// this only looks one step into the future....

//