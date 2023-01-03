#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 60

int get_letter_priority(char letter);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: ./rucksack filename\n");
        return 1;
    }

    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    char current_rucksack[BUFFER_SIZE];

    int priority_sum = 0;

    while (fgets(current_rucksack, BUFFER_SIZE, fp))
    {
        int items_in_rucksack = 0;

        while (current_rucksack[items_in_rucksack] != '\n' && current_rucksack[items_in_rucksack] != 0)
        {
            items_in_rucksack++;
        }

        int half_rucksack = items_in_rucksack / 2;

        int has_found_matching_char = 0;
        int i = 0;

        while (i < half_rucksack && !has_found_matching_char)
        {
            char compartment_one_char = current_rucksack[i];

            for (int j = half_rucksack; j < items_in_rucksack; j++)
            {
                char compartment_two_char = current_rucksack[j];

                if (compartment_one_char == compartment_two_char)
                {
                    priority_sum += get_letter_priority(compartment_one_char);
                    has_found_matching_char = 1;
                    break;
                }
            }
            i++;
        }
        has_found_matching_char = 0;
    }

    fclose(fp);

    printf("%i", priority_sum);

    return 0;
}

int get_letter_priority(char letter)
{
    if (isupper(letter))
    {
        return letter - 38;
    }

    return letter - 96;
}