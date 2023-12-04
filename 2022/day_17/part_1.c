#include <stdio.h>

// create the different rock types

int main(int argc, char **argv)
{
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    return 0;
}