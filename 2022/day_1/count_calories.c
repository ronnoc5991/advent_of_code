#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LENGTH 256

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: ./count_calories filename number_of_elves_to_count\n");
        return 1;
    }

    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s\n", filename);
        return 1;
    }

    int elves_to_count = atoi(argv[2]);
    int highest_calorie_counts[elves_to_count];

    for (int i = 0; i < elves_to_count; i++) {
        highest_calorie_counts[i] = 0;
    }

    int current_elf_calorie_count = 0;
    char current_line[MAX_LENGTH];

    while (fgets(current_line, MAX_LENGTH, fp)) 
    {
        if (current_line[0] == '\n') // end of previous elf
        {
            for (int i = 0; i < elves_to_count; i++)
            {
                if (current_elf_calorie_count > highest_calorie_counts[i])
                {
                    for (int j = elves_to_count - 1; j > i; j--)
                    {
                        highest_calorie_counts[j] = highest_calorie_counts[j - 1];
                    }

                    highest_calorie_counts[i] = current_elf_calorie_count;
                    break;
                }
            }
            current_elf_calorie_count = 0;
        }
        else
        {
            current_elf_calorie_count += atoi(current_line);
        }
    }

    fclose(fp);

    int sum_of_calorie_counts = 0;

    for (int i = 0; i < elves_to_count; i++)
    {
        sum_of_calorie_counts += highest_calorie_counts[i];
    }

    printf("Top %i elves calorie counts summed: %d\n", elves_to_count, sum_of_calorie_counts);

    return 0;
}