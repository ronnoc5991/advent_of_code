#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE 40
#define EXPECTED_ARG_COUNT 3
#define FILENAME_ARG_INDEX 1
#define NUMBER_OF_RACES_ARG_INDEX 2

int main(int argc, char *argv[])
{
    if (argc != EXPECTED_ARG_COUNT) {
        printf("Incorrect input. Exiting.\n");
        return -1;
    }

    FILE *fptr;

    fptr = fopen(argv[FILENAME_ARG_INDEX], "r");

    if (fptr == NULL) {
        printf("Unable to open file.  Exiting.\n");
        fclose(fptr);
        return -1;
    }

    int number_of_races = atoi(argv[NUMBER_OF_RACES_ARG_INDEX]);

    int times[number_of_races];
    int distances[number_of_races];

    char line[MAX_LINE];

    fgets(line, MAX_LINE, fptr); // read the times line into line

    int c;
    int index = 0; // index in the times/distances arrays
    int in_number = 0;
    int current_number = 0;

    for (int i = 0; i < MAX_LINE; i++) {
        c = line[i];

        if (isdigit(c)) {
            in_number = 1;
            current_number = current_number * 10 + (c - '0');
            continue;
        }

        if (c == ' ' || c == '\n') {
            if (in_number) { // if we were reading a number
                in_number = 0;
                times[index] = current_number;
                current_number = 0;
                index++;
            }
            continue;
        }
    }

    index = 0;
    in_number = 0;
    current_number = 0;

    fgets(line, MAX_LINE, fptr); // read the times line into line

    for (int l = 0; l < MAX_LINE; l++) {
        c = line[l];

        if (isalpha(c))
            continue;

        if (isdigit(c)) {
            in_number = 1;
            current_number = current_number * 10 + (c - '0');
            continue;
        }

        if (c == ' ' || c == '\n' || c == EOF || c == '\0') {
            printf("%d - %d\n", c, current_number);
            if (in_number) { // if we were reading a number
                in_number = 0;
                distances[index] = current_number;
                current_number = 0;
                index++;
            }
            continue;
        }
    }

    printf("      Time  Distance\n");

    for (int i = 0; i < number_of_races; i++) {
        printf("Race #%d: %d - %d\n", i + 1, times[i], distances[i]);
    }

    return 0;
}