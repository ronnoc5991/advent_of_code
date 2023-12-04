#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//                 [V]     [C]     [M]
// [V]     [J]     [N]     [H]     [V]
// [R] [F] [N]     [W]     [Z]     [N]
// [H] [R] [D]     [Q] [M] [L]     [B]
// [B] [C] [H] [V] [R] [C] [G]     [R]
// [G] [G] [F] [S] [D] [H] [B] [R] [S]
// [D] [N] [S] [D] [H] [G] [J] [J] [G]
// [W] [J] [L] [J] [S] [P] [F] [S] [L]
//  1   2   3   4   5   6   7   8   9

// NOW: the crane moves all of the string in the same order that they are in
// grab the last crane_count chars from the stack
// concatenate them onto the new stack

void move_crates(int number_of_crates, int from_stack, int to_stack);

const int stack_count = 9;

char stacks[9][100] = {
    {"WDGBHRV"},
    {"JNGCRF"},
    {"LSFHDNJ"},
    {"JDSV"},
    {"SHDRQWNV"},
    {"PGHCM"},
    {"FJBGLZHC"},
    {"SJR"},
    {"LGSRBNVM"},
};

int main(void)
{

    char *filename = "input.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s", filename);
        return 1;
    }

    char current_operation[30];

    while (fgets(current_operation, 30, fp))
    {
        char new_string[10];

        for (int i = 0; i < 10; i++)
        {
            new_string[i] = 0;
        }

        int i = 0;
        int last_char_was_int = 0;

        while (current_operation[i] != 0)
        {
            if (isdigit(current_operation[i]))
            {
                new_string[strlen(new_string)] = current_operation[i];
                last_char_was_int = 1;
            }
            else if (last_char_was_int)
            {
                new_string[strlen(new_string)] = ',';
                last_char_was_int = 0;
            }

            i++;
        }

        char *digit = strtok(new_string, ",");

        int crate_count;
        int from_stack;
        int to_stack;

        int j = 0;

        while (digit != NULL)
        {
            if (j == 0)
            {
                crate_count = atoi(digit);
            }
            else if (j == 1)
            {
                from_stack = atoi(digit);
            }
            else if (j == 2)
            {
                to_stack = atoi(digit);
            };

            digit = strtok(NULL, ",");
            j++;
        }

        move_crates(crate_count, from_stack - 1, to_stack - 1);
    }

    for (int i = 0; i < stack_count; i++)
    {
        printf("%s\n", stacks[i]);
    }

    return 0;
}

void move_crates(int number_of_crates, int from_stack, int to_stack)
{
    int length_of_from_stack = strlen(stacks[from_stack]);

    for (int i = 0; i < number_of_crates; i++)
    {
        char crate = stacks[from_stack][length_of_from_stack - number_of_crates + i];

        int length_of_to_stack = strlen(stacks[to_stack]);

        stacks[to_stack][length_of_to_stack] = crate;

        stacks[from_stack][length_of_from_stack - number_of_crates + i] = 0;
    }
}