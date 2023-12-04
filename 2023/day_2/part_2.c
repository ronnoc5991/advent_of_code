#include <stdio.h>
#include <string.h>
#include <ctype.h>

// The Elf would first like to know which games would have been possible if the bag contained only 12 red cubes, 13 green cubes, and 14 blue cubes?

// for each game
// get the max for each color
// multiply those maxes together
// add them to the sum

// a place to store the sum
// a place to store the max for each color for each game
// 

#define MAX_LINE_LENGTH 165
#define NUMBER_OF_COLORS 3

int digit_to_int(char digit)
{
    return digit - '0';
}

char colors[NUMBER_OF_COLORS] = { 'r', 'g', 'b' };

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

    while (fgets(line, MAX_LINE_LENGTH, fptr)) { // for each game
        long line_length = strlen(line);
        int have_reached_data = 0;
        int have_seen_first_letter = 0;
        //                 r  g  b
        int game_maxes[NUMBER_OF_COLORS] = { 0, 0, 0 };
        int count = 0;
        char c;

        // for each character
        for (int i = 0; i < line_length; i++) {
            c = line[i];

            if (c == '\n')
                break;

            if (c == ' ')
                continue;

            // go until we reach the semi colon
            if (have_reached_data == 0) {
                if (c == ':')
                    have_reached_data = 1;

                continue;
            }

            if (c == ',' || c == ';') { // we reached a new color
                count = 0;
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

            for (int j = 0; j < NUMBER_OF_COLORS; j++) { // for every color
                if (c != colors[j])
                    continue;

                if (count > game_maxes[j])
                    game_maxes[j] = count;

                have_seen_first_letter = 1;
                break;
            }


        }

        int power_of_set = 1;

        for (int k = 0; k < NUMBER_OF_COLORS; k++) {
            power_of_set = power_of_set * game_maxes[k];
        }
        
        sum += power_of_set;
    }

    printf("Answer: %d\n", sum);
}