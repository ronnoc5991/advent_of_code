#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 25

int get_decimal_value(char *string);
int snafu_to_decimal(char snafu);
void get_snafu_value(int decimal);

int main (int argc, char** argv)
{
    // if (argc != 2) {
    //     printf("Use: ./part_1 filename\n");
    //     return 1;
    // }

    // char *filename = argv[1];
    char *filename = "test.txt";
    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Unable to open file: %s\n", filename);
        return 2;
    }

    char buffer[MAX_LENGTH];
    int sum = 0;


    while(fgets(buffer, MAX_LENGTH, fp) != NULL) {
        int length = strlen(buffer);
        if (buffer[length - 1] == '\n')
            buffer[length -1] = '\0';

        sum += get_decimal_value(buffer);
    }

    printf("%i\n", sum);

    get_snafu_value(sum);

    return 0;
}

int snafu_to_decimal(char snafu) {
    switch (snafu)
    {
    case '=':
        return -2;
    case '-':
        return -1;
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    
    default:
        return 0;
    }
}

int get_decimal_value(char *string) {
    int decimal_value = 0;
    int place_value = 1;

    while (string[0] != '\0') {
        int length = strlen(string);
        char last_char = string[length - 1];
        int decimal_digit = snafu_to_decimal(last_char);
        decimal_value += place_value * decimal_digit;
        place_value *= 5;
        string[length - 1] = '\0';
    }

    return decimal_value;
}

void get_snafu_value(int decimal) {
    char *digits = malloc(sizeof(char) * 2);

    if (digits == NULL)
        return;

    digits[0] = '0';
    digits[1] = '\0';

    if (decimal == 0)
        return;

    int place_value = 1;
    int number_of_digits = 0;
    int current_index = 0;
    int target_value = decimal;

    while (1) {
        number_of_digits++;

        if (target_value > 0) {
            if (place_value * 2 >= target_value) {
                int min_value = 0;
                for (int i = place_value / 5; i > 0; i/=5) {
                    min_value += i * -2;
                }

                char digit_to_add;

                // do the remaining digits offset the value we would be adding here
                // -1000 > -800
                if (target_value - (place_value * 2) <= min_value) {
                    digit_to_add = '1';
                }
                else {
                    digit_to_add = '2';
                }
                digits = realloc(digits, sizeof(char) * (number_of_digits + 1));
                digits[current_index] = digit_to_add;
                current_index++;
                target_value -= place_value * (digit_to_add == '1' ? 1 : 2);
                break;
            }
        } else {
            if (place_value * -2 <= target_value) {
                int max_value = 0;
                for (int i = place_value / 5; i > 0; i/=5) {
                    max_value += i * 2;
                }

                char digit_to_add;
                if (target_value - (place_value * 2) > max_value) {
                    digit_to_add = '-';
                }
                else {
                    digit_to_add = '=';
                }

                digits = realloc(digits, sizeof(char) * (number_of_digits + 1));
                digits[current_index] = digit_to_add;
                current_index++;
                target_value -= place_value * (digit_to_add == '-' ? -1 : -2);
                break;
            }
        }
        place_value *= 5;
    }

    place_value /= 5;

    while (place_value > 0) {
        char digit_to_add = '0';

        if (target_value > 0) {
            if (place_value * 2 >= target_value) {
                int min_value = 0;
                for (int i = place_value / 5; i > 0; i/=5) {
                    min_value += i * -2;
                }

                if (target_value - (place_value * 2) <= min_value) { // if a two is too big
                    digit_to_add = '1';
                }
                else {
                    digit_to_add = '2';
                }
                target_value -= place_value * (digit_to_add == '1' ? 1 : 2);
            }
        } // if I put a number here, will we be able to overcome it with the remaining digits
        else { // -1250 <= -1360
            // we are trying to get a negative number
            // should we put a -1 or -2 here?
            int max_value = 0;
            for (int i = place_value / 5; i > 0; i/=5) {
                max_value += i * 2;
            }

            if (target_value - (place_value * -2) > -max_value) {
                digit_to_add = '=';
            }
            else if (target_value - (place_value * -1) > -max_value) {
                digit_to_add = '-';
            }

            target_value -= place_value * (digit_to_add == '-' ? -1 : -2);
        }
        digits[current_index] = digit_to_add;
        place_value /= 5;
        current_index++;
    }
    digits[number_of_digits - 1] = '\0';
    printf("%i -  %s\n",number_of_digits, digits);
}