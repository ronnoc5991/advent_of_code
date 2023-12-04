#include <stdio.h>
#include <ctype.h>

// https://adventofcode.com/2023/day/1

#define BUFFER_SIZE 100
#define UNINITIALIZED -1

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
    int c;
    char buffer[BUFFER_SIZE];

    fptr = fopen(argv[1], "r");

    if (fptr == NULL) {
        printf("Unable to open the file\n");
        fclose(fptr);
        return 1;
    }

    // for each line of input
    while(fgets(buffer, BUFFER_SIZE, fptr)) {
        int first_int;
        int second_int;

        first_int = second_int = UNINITIALIZED;

        // for each character in the line
        for (int i = 0; i < BUFFER_SIZE; i++) {
            c = buffer[i];

            if (c == '\n') {
                int tens;
                int ones;

                tens = first_int * 10;

                if (second_int == UNINITIALIZED) {
                    ones = first_int;
                } else {
                    ones = second_int;
                }
                
                int local_sum = tens + ones;

                printf("Calibration value: %d\n", local_sum);

                sum = sum + local_sum;
                
                break;
            }

            if (isdigit(c) == 0)
                continue;

            if (first_int == UNINITIALIZED) {
                first_int = c - '0'; // convert c to int
            } else {
                second_int = c - '0';
            }
        }
    }
    
    printf("\n\n");

    printf("Sum of calibration values: %d\n", sum);

    return 0;
}