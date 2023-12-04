#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// https://adventofcode.com/2023/day/1#part2

#define BUFFER_SIZE 100
#define UNINITIALIZED -1
#define CHARACTERS_IN_ALPHABET 26
#define NUMBER_OF_WORDS 9
#define WORD_LENGTH 6

typedef struct node {
    int integer_value;
    struct node *children[CHARACTERS_IN_ALPHABET];
} node;

// root of trie
node *dictionary = NULL;

node *create_node()
{
    node *new_node = malloc(sizeof(node));
    new_node->integer_value = 0;

    for (int i = 0; i < CHARACTERS_IN_ALPHABET; i++) {
        new_node->children[i] = NULL;
    }

    return new_node;
}

int hash(char c)
{
    return c - 'a';
}

int get_integer_value(char *character, node *root)
{
    printf("Entering get integer value with %c\n", *character);
    if (*character == '\0' || *character == '\n' || *character == EOF || *character < 'a' || *character > 'z') return 0;

    int index = hash(*character);

    if (root->children[index] == NULL)
        return 0;
    else if (root->children[index]->integer_value != 0)
        return root->children[index]->integer_value;
    else
        return get_integer_value(++character, root->children[index]);
}

char words[NUMBER_OF_WORDS][WORD_LENGTH] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
int integers[NUMBER_OF_WORDS] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

int main(int argc, char *argv[])
{
    if (argc == 1) {
        printf("Please provide input file as argument.\n");
        return 1;
    } else if (argc > 2) {
        printf("Too many arguments. Expected a single input file as argument.\n");
        return 1;
    }

    dictionary = create_node();

    // load words into dictionary
    for (int i = 0; i < NUMBER_OF_WORDS; i++) {
        node *temp = dictionary;
        int c;

        for (char *c = words[i]; *c != '\0'; c++) {
            int index = hash(*c);

            if (temp->children[index] == NULL) {
                node *new_node = create_node();
                temp->children[index] = new_node;
            }

            temp = temp->children[index];
        }

        temp->integer_value = integers[i];
    }


    FILE *fptr;
    int sum = 0;
    char c;
    char buffer[BUFFER_SIZE];

    fptr = fopen(argv[1], "r");

    if (fptr == NULL) {
        printf("Unable to open the file\n");
        fclose(fptr);
        return 1;
    }

    // for each line of input
    while(fgets(buffer, BUFFER_SIZE, fptr)) {
        long word_length = strlen(buffer);
        int first_int;
        int second_int;

        first_int = second_int = UNINITIALIZED;

        // for each character in the line
        for (int i = 0; i < word_length; i++) {
            c = buffer[i];

            if (c == '\n' || c == EOF) { // we reached the end of the line
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

            if (isdigit(c) == 1) {
                if (first_int == UNINITIALIZED) {
                    first_int = c - '0'; // convert c to int
                } else {
                    second_int = c - '0';
                }
            } else if (c >= 'a' && c <= 'z') {

                int value = get_integer_value(&buffer[i], dictionary);


                if (value != 0) {
                    printf("Value: %d\n", value);
                    if (first_int == UNINITIALIZED) {
                        first_int = value;
                    } else {
                        second_int = value;
                    }
                }
            }
        }
    }
    
    printf("\n\n");

    printf("Sum of calibration values: %d\n", sum);

    return 0;
}