#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define BUFFER_SIZE 60

int get_letter_priority(char letter);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: ./rucksack_2 filename\n");
        return 1;
    }

    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    int priority_sum = 0;

    int group_count = 0;

    char first_elf_rucksack[BUFFER_SIZE];
    char second_elf_rucksack[BUFFER_SIZE];
    char third_elf_rucksack[BUFFER_SIZE];

    int has_found_group_badge = 0;

    while (fgets(first_elf_rucksack, BUFFER_SIZE, fp))
    {
        fgets(second_elf_rucksack, BUFFER_SIZE, fp);
        fgets(third_elf_rucksack, BUFFER_SIZE, fp);

        int i = 0;

        while (first_elf_rucksack[i] != '\n' && first_elf_rucksack[i] != 0 && !has_found_group_badge)
        {
            char first_elf_character = first_elf_rucksack[i];

            int j = 0;
            while (second_elf_rucksack[j] != '\n' && second_elf_rucksack[j] != 0 && !has_found_group_badge)
            {
                char second_elf_character = second_elf_rucksack[j];

                if (first_elf_character == second_elf_character)
                {
                    int k = 0;

                    while (third_elf_rucksack[k] != '\n' && third_elf_rucksack[k] != 0 && !has_found_group_badge)
                    {
                        char third_elf_character = third_elf_rucksack[k];

                        if (third_elf_character == second_elf_character)
                        {
                            priority_sum += get_letter_priority(third_elf_character);
                            has_found_group_badge = 1;
                            break;
                        }

                        k++;
                    }
                }
                j++;
            }
            i++;
        }

        has_found_group_badge = 0;
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