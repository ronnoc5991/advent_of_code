#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 10

typedef struct {
    int number;
    int index;
} indexed_number;

void parse_numbers(FILE *fp, indexed_number **numbersp, int *number_count);
void print_numbers(indexed_number *numbers, int number_count);
int get_current_index(indexed_number *numbers, int number_count, int original_index);
// given a list of numbers
// proceed through the list
// moving each found nuber either right or left the number of indices equal to its number
// they should be move in the same order that they are found

// i have a long list of numbers
// i need to remember what order they WERE in before i moved them
// i need to visit each one once

// could read a number, store it in the number array, then store a pointer to it in another array
// but then I think moving the number in the number array would fuck up the pointer?

// that would involve a couple of arrays and iterating through them

// could also store each number as a struct
// store the number, along with its original index
// then, find next index number in array, move it, repeat?
// this involves searching the array a lot (and it is not sorted!)

int main(int argc, char **argv)
{
    if (argv[1] == NULL)
    {
        printf("Please provide a file to process!\n");
        return 1;
    }

    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 2;
    }

    int number_count = 0;
    indexed_number *numbers = malloc(sizeof(*numbers) * number_count);

    parse_numbers(fp, &numbers, &number_count);
    print_numbers(numbers, number_count);

    return 0;
}

void parse_numbers(FILE *fp, indexed_number **numbersp, int *number_countp)
{
    char current_line[MAX_LINE_LENGTH];

    while(fgets(current_line, MAX_LINE_LENGTH, fp) != NULL)
    {
        indexed_number new_number;
        new_number.number = atoi(current_line);
        new_number.index = *number_countp;
        (*number_countp)++;

        *numbersp = realloc(*numbersp, sizeof(*numbersp) * *number_countp);
        (*numbersp)[*number_countp - 1] = new_number;
    }
}

void print_numbers(indexed_number *numbers, int number_count)
{
  for (int i = 0; i < number_count; i++)
    {
        printf("%i - %i\n", numbers[i].index, numbers[i].number);
    }
}

int get_current_index(indexed_number *numbers, int number_count, int original_index)
{
    int current_index;
    for (int i = 0; i < number_count; i++)
    {
        if (numbers[i].index == original_index)
        {
            current_index = numbers[i].index;
            break;
        }
    }

    return current_index;
}

int relocate_number(indexed_number *numbers, int number_count, int current_index)
{
    int numbers_to_skip = numbers[current_index].number; // this tells us how many numbers to skip and in what direction
    
    // we have the amount of numbers to skip, and the direction to skip them in...
    // if we skip numbers to the left and end at 0, just wrap to the end
    

}

