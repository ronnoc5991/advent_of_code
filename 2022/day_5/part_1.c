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

void move_crate(int from_stack, int to_stack);

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
        int from_crate;
        int to_crate;

        int j = 0;

        while (digit != NULL)
        {
            if (j == 0)
            {
                crate_count = atoi(digit);
            }
            else if (j == 1)
            {
                from_crate = atoi(digit);
            }
            else if (j == 2)
            {
                to_crate = atoi(digit);
            };

            digit = strtok(NULL, ",");
            j++;
        }

        for (int i = 0; i < crate_count; i++)
        {
            move_crate(from_crate - 1, to_crate - 1);
        }
    }

    for (int i = 0; i < stack_count; i++)
    {
        printf("%s\n", stacks[i]);
    }

    return 0;
}

void move_crate(int from_stack, int to_stack)
{
    int from_length = strlen(stacks[from_stack]);
    char top_crate = stacks[from_stack][from_length - 1];
    stacks[from_stack][from_length - 1] = 0; // erase crate from from_stack

    int to_length = strlen(stacks[to_stack]);
    stacks[to_stack][to_length] = top_crate;
}