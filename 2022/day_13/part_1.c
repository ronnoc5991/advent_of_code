#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 205
#define BURN_LINE_LENGTH 2

char *get_next_line(char *line, int max_length, FILE *fp);
int are_lines_in_correct_order(char *pLeft, char *pRight);
int compare_lists(char *left, char *right);

int main(void)
{
    char *filename = "input.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    char left[MAX_LINE_LENGTH];
    char right[MAX_LINE_LENGTH];
    char burn_line[BURN_LINE_LENGTH];

    int group_index = 1;
    int sum_of_group_indices = 0;

    while (get_next_line(left, MAX_LINE_LENGTH, fp) != NULL)
    {
        get_next_line(right, MAX_LINE_LENGTH, fp);

        if (are_lines_in_correct_order(left, right))
        {
            sum_of_group_indices += group_index;
        }

        get_next_line(burn_line, BURN_LINE_LENGTH, fp);
        group_index++;
    }

    return 0;
}

char *get_next_line(char *line, int max_length, FILE *fp)
{
    char *buffer = fgets(line, max_length, fp);

    int length = strlen(line);

    if (line[length - 1] == '\n')
    {
        line[length - 1] = 0;
    }

    return buffer;
}

int are_lines_in_correct_order(char *pLeft, char *pRight)
{
    // here, we have two lists
    // we could pass them to compare lists
    char current_left = *pLeft;
    char current_right = *pRight;

    while (current_left != 0 && current_right != 0)
    {

        // if we get one opening bracket and one int, treat the int as if it were in a list?
        // how do we know when that list should end?

        // the simplest unit is a list
        // we should find the lists, then call compare lists
        // compare lists should compare the lists until a new list starts
        // then it should call compare lists with that sublist

        current_left = pLeft++;
        current_right = *pRight++;
    }

    return 1;

    // if one of the chars is 0, decide what to do

    // need to get the first 'group' from each list

    // if both characters are integers, we can compare them
    // if left integer is smaller, they are in the right order
    // if right integer is higher than left integer, this is not in the right order

    // if both values are lists
    // compare each item in the list
    // if the right list runs out first, they are not in the right order

    // if one of the values is a list and the other is not, convert the not list into a list
}

int compare_lists(char *left, char *right)
{
    char left_char = *left;
    char right_char = *right;

    if (left_char != '[' || right_char != '[')
    {
        printf("List did not start with [\n");
        return 0;
    }

    left_char = *left++;
    right_char = *right++;

    if (isnumber(left_char) && isnumber(right_char))
    {
        if (left_char < right_char)
            return 1;
        else if (left_char > right_char)
            return 0;
    }
    else if (left_char == '[' && right_char == '[')
    {
        // we need to compare the lists
        compare_lists(left, right);
    }
    else if (left_char == '[' && isnumber(right_char))
    {
        // need to convert right values
    }
    // if left is a list but right is not
    // if right is a list but left is not
    // if both are commas
    // if left is a comma but right is not
    // if right is a comma but left is not

    // when we find a new list, create a substring from each line and pass them into this function again?
}

// if the first char is a number, that is the value
// if the first char is a new list, we need to find the end of that list, the resulting list is the value

// [[1],2,3]
// [1,[2],3]

// if we get an int and a list, just strip the brackets from the list and compare?

// we are given a list

// a list is made up of values
// a value can be an integer
// a value can be a list

char *get_value_from_list(char *list)
{
    // if the second char is an integer, the value is an integer
    if (isnumber(list[1]))
    {
        }
    // if the second char is an opening bracket, we need to find the closing bracket, and return everything in between
}