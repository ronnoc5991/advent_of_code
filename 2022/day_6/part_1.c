#include <stdio.h>

#define NUMBER_OF_CHARS 14

int main(void)
{
    char filename[] = "input.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s", filename);
        return 1;
    }

    char current_char;
    int char_count = 0;
    char last_fourteen_chars[NUMBER_OF_CHARS + 1];

    for (int i = 0; i < NUMBER_OF_CHARS + 1; i++)
    {
        last_fourteen_chars[i] = 0;
    }

    while ((current_char = fgetc(fp)) != EOF)
    {
        last_fourteen_chars[char_count % NUMBER_OF_CHARS] = current_char;
        char_count++;

        if (char_count < NUMBER_OF_CHARS)
        {
            continue;
        }

        int is_duplicate_char_found = 0;

        for (int i = 0; i < NUMBER_OF_CHARS; i++)
        {
            for (int j = i + 1; j < NUMBER_OF_CHARS; j++)
            {
                if (last_fourteen_chars[i] == last_fourteen_chars[j])
                {
                    is_duplicate_char_found = 1;
                }
            }
        }

        if (!is_duplicate_char_found)
        {
            break;
        }
    }

    printf("%i", char_count);

    return 0;
}