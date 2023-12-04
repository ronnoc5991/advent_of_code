#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INSTRUCTION_LENGTH 10

int is_special_cyle(int cycle_count);

int main(void)
{
    char *filename = "input.txt";
    FILE *fp = fopen(filename, "r");

    char current_instruction[MAX_INSTRUCTION_LENGTH];

    int clock_cycle = 0;
    int register_x = 1;
    int cumulative_signal_strength = 0;

    while (fgets(current_instruction, MAX_INSTRUCTION_LENGTH, fp) != NULL)
    {
        int cycles_to_wait = 0;
        int value_to_add = 0;

        if (current_instruction[strlen(current_instruction) - 1] == '\n')
        {
            current_instruction[strlen(current_instruction) - 1] = 0;
        }

        char *op_code = strtok(current_instruction, " ");

        if (strcmp(op_code, "noop") == 0)
        {
            cycles_to_wait = 1;
        }
        else
        {
            cycles_to_wait = 2;
            value_to_add = atoi(strtok(NULL, " "));
        }

        for (int i = 0; i < cycles_to_wait; i++)
        {
            clock_cycle++; // 1

            if (is_special_cyle(clock_cycle))
            {
                cumulative_signal_strength += clock_cycle * register_x;
            }
        }

        register_x += value_to_add;
    }

    printf("%i\n", cumulative_signal_strength);

    return 0;
}

int is_special_cyle(int cycle_count)
{
    switch (cycle_count)
    {
    case 20:
    case 60:
    case 100:
    case 140:
    case 180:
    case 220:
        return 1;
    default:
        return 0;
    }
}