#include <stdio.h>
// winning numbers | the numbers you have

// find out how many of the numbers you have are winning numbers
// for each of those, calculate the point value for that card
// the first card is worth 1 point
// each card after that doubles the point value

#define MAX_LINE 120
#define NUMBER_OF_WINNING_NUMBERS 10
#define NUMBER_OF_PLAYER_NUMBERS 25

int calculate_score(int number_of_winners);
int parse_number(char s[], int length);
void parse_winning_numbers(char s[], int numbers[]);

int main(int argc, char *argv[])
{
    if (argc < 1) {
        printf("Too few arguments.\n");
        return 1;
    } else if (argc > 2) {
        printf("Too many arguments.\n");
        return 1;
    }

    FILE *fptr;

    fptr = fopen(argv[1], "r");

   if (fptr == NULL) {
        printf("Unable to open the file\n");
        fclose(fptr);
        return 1;
    }

    char line[MAX_LINE];
    int total = 0;

    while(fgets(line, MAX_LINE, fptr)) {
        int i, c, matching_numbers;
        int winning_numbers[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

        matching_numbers = 0;

        for (i = 0; (c = line[i]) != ':'; i++);
        
        i++; // skip ':'
        i++; // skip ' '

        parse_winning_numbers(&line[i], winning_numbers);

        while ((c = line[i])) {
            if (c == '|') {
                i += 2;
                break;
            }
            i++;
            continue;
        }

        for (int j = 0; j < NUMBER_OF_PLAYER_NUMBERS; j++) {
            int number = parse_number(&line[i], 2);
            for (int k = 0; k < NUMBER_OF_WINNING_NUMBERS; k++) {
                if (number == winning_numbers[k]) {
                    matching_numbers++;
                    break;
                }
            }
            i += 3;
        }

        int score = calculate_score(matching_numbers);

        total += score;
    }

    printf("Total: %d\n", total);

    return 0;
}

int calculate_score(int number_of_winners)
{
    int score;
    for (score = 0; number_of_winners > 0; number_of_winners--) {
        if (score > 0)
            score = score * 2;
        else
            score = 1;
    }
    return score;
}

void parse_winning_numbers(char s[], int numbers[])
{
    for (int i = 0; i < NUMBER_OF_WINNING_NUMBERS; i++) {
        int winning_number = parse_number(s, 2);
        numbers[i] = winning_number;
        s+=3; // skip to next number
    }
}

int parse_number(char s[], int length)
{
    int c;
    int number = 0;

    for (int i = 0; i < length; i++) {
        c = s[i];

        if (c == ' ')
            continue;
        if (c >= '0' || c <= '9') {
            if (number == 0)
                number += c - '0';
            else
                number = (number * 10) + (c - '0');
        }
    }

    return number;
}