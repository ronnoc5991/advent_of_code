#include <stdio.h>
#include <string.h>
#include <ctype.h>

// The Elf would first like to know which games would have been possible if the bag contained only 12 red cubes, 13 green cubes, and 14 blue cubes?

#define MAX_LINE_LENGTH 165
#define NUMBER_OF_COLORS 3

int digit_to_int(char digit)
{
    return digit - '0';
}

char colors[NUMBER_OF_COLORS] = { 'r', 'g', 'b' };
int limits[NUMBER_OF_COLORS] = { 12, 13, 14 };

int main(int argc, char *argv[])
{
    if (argc == 1) {
        printf("Please provide input file as argument.\n");
        return 1;
    } else if (argc > 2) {
        printf("Too many arguments. Expected a single input file as argument.\n");
        return 1;
    }

    FILE *fptr;
    int sum = 0;
    char line[MAX_LINE_LENGTH];

    fptr = fopen(argv[1], "r");

    if (fptr == NULL) {
        printf("Unable to open the file\n");
        fclose(fptr);
        return 1;
    }

    int line_number = 0;

    while (fgets(line, MAX_LINE_LENGTH, fptr)) {
        line_number++;

        long line_length = strlen(line);
        int have_reached_data = 0;
        int count = 0;
        int is_count_too_much = 0;
        int have_seen_first_letter = 0;
        char c;

        // for each character
        for (int i = 0; i < line_length; i++) {
            c = line[i];

            if (c == '\n')
                break;

            // go until we reach the semi colon
            if (have_reached_data == 0) {
                if (c == ':')
                    have_reached_data = 1;

                continue;
            }

            if (c == ' ')
                continue;

            if (c == ',' || c == ';') {
                count = 0;
                is_count_too_much = 0;
                have_seen_first_letter = 0;
                continue;
            }

            if (have_seen_first_letter == 1)
                continue;

            if (isdigit(c)) {
                printf("Found digit: %d\n", c - '0');
                count = (count * 10) + digit_to_int(c);
                continue;
            }

            for (int j = 0; j < NUMBER_OF_COLORS; j++) {
                if (c != colors[j])
                    continue;
                
                printf("Color: %c -> Count: %d -> Limit: %d\n", c, count, limits[j]);

                if (count > limits[j])
                    is_count_too_much = 1;

                have_seen_first_letter = 1;
                break;
            }

            if (is_count_too_much == 1) {
                printf("Count is too much! Count: %d -> Color: %c\n", count, c);
                break;
            }// this game could not have happened
        }

        if (is_count_too_much == 1)
            continue;

        sum += line_number;
    }

    printf("Answer: %d\n", sum);
}