#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// elves are split into pairs
// their ranges are given on a single line
// for each pair, need to check if one of the ranges of ids includes the other range completely
// count the number of assignment pairs where one assignment completely includes the other

typedef struct
{
    int start;
    int end;
} assignment;

assignment get_assignment(char *range);
int does_assignment_contain_assignment(assignment assignment_one, assignment assignment_two);

int main(void)
{
    char filename[] = "input.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s", filename);
        return 1;
    }

    char *current_line = malloc(20);

    int count = 0;

    while (fgets(current_line, 20, fp))
    {
        char *first = strtok(current_line, ",");
        char *second = strtok(NULL, ",");

        assignment first_assignment = get_assignment(first);
        assignment second_assignment = get_assignment(second);

        if (does_assignment_contain_assignment(first_assignment, second_assignment))
        {
            count++;
        }
    }

    printf("%i", count);

    return 0;
}

assignment get_assignment(char *range)
{
    assignment new_assignment;

    new_assignment.start = atoi(strtok(range, "-"));
    new_assignment.end = atoi(strtok(NULL, "-"));

    return new_assignment;
}

int does_assignment_contain_assignment(assignment assignment_one, assignment assignment_two)
{
    if ((assignment_one.start <= assignment_two.start && assignment_one.end >= assignment_two.end) || (assignment_two.start <= assignment_one.start && assignment_two.end >= assignment_one.end))
    {
        return 1;
    }

    return 0;
}