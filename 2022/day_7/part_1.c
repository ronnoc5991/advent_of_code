#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 30
#define MAX_NAME_LENGTH 20
#define MAX_DIRECTORY_SIZE 100000

struct directory *create_new_directory(char *name, struct directory *parent_directory);
void save_directory_name(char *new_directory_name, char *name);
void check_directory_size(struct directory *directory, int *sumptr);

struct directory
{
    char name[MAX_NAME_LENGTH];
    struct directory *parent_directory;
    struct directory **child_directories;
    int number_of_child_directories;
    int size_of_files;
};

int main(void)
{
    char filename[] = "input.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s", filename);
        return 1;
    }

    char current_line[MAX_LINE_LENGTH];

    struct directory *pwdptr;
    struct directory *home_directory = create_new_directory("/", NULL);

    while (fgets(current_line, MAX_LINE_LENGTH, fp) != NULL)
    {
        current_line[strlen(current_line) - 1] = 0; // remove newline chars from strings

        char *first_token = strtok(current_line, " ");
        char *second_token = strtok(NULL, " ");
        char *third_token = strtok(NULL, " ");

        if (strcmp(first_token, "$") == 0)
        {
            if (strcmp(second_token, "cd") == 0)
            {
                if (strcmp(third_token, "/") == 0)
                {
                    pwdptr = home_directory;
                }
                else if (strcmp(third_token, "..") == 0)
                {
                    pwdptr = pwdptr->parent_directory;
                }
                else
                {
                    for (int i = 0; i < pwdptr->number_of_child_directories; i++)
                    {
                        if (strcmp(pwdptr->child_directories[i]->name, third_token) == 0)
                        {
                            pwdptr = pwdptr->child_directories[i];
                        }
                    }
                }
            }
        }
        else if (strcmp(first_token, "dir") == 0)
        {
            struct directory *new_directory = create_new_directory(second_token, pwdptr);

            pwdptr->number_of_child_directories++;

            struct directory **new_child_directories = malloc(pwdptr->number_of_child_directories * sizeof(struct directory));

            for (int i = 0; i < pwdptr->number_of_child_directories; i++)
            {
                struct directory *child_directory = i == pwdptr->number_of_child_directories - 1 ? new_directory : pwdptr->child_directories[i];
                new_child_directories[i] = child_directory;
            }

            pwdptr->child_directories = new_child_directories;
        }
        else
        {
            int file_size = atoi(first_token);
            pwdptr->size_of_files += file_size;

            struct directory *pwdptrcopy = pwdptr;

            while (pwdptrcopy->parent_directory != NULL)
            {
                pwdptrcopy = pwdptrcopy->parent_directory;
                pwdptrcopy->size_of_files += file_size;
            }
        }
    }

    int sum = 0;

    check_directory_size(home_directory, &sum);

    printf("%i", sum);

    return 0;
}

struct directory *create_new_directory(char *name, struct directory *parent_directory)
{
    struct directory *new_directory = malloc(sizeof(struct directory));

    save_directory_name(new_directory->name, name);
    new_directory->parent_directory = parent_directory;
    new_directory->size_of_files = 0;
    new_directory->number_of_child_directories = 0;

    return new_directory;
}

void save_directory_name(char *new_directory_name, char *name)
{
    int name_length = strlen(name);

    for (int i = 0; i < MAX_NAME_LENGTH; i++)
    {
        int next_char = i < name_length ? name[i] : 0;

        new_directory_name[i] = next_char;
    }
}

void check_directory_size(struct directory *directory, int *sumptr)
{
    if (directory->size_of_files < MAX_DIRECTORY_SIZE)
    {
        *sumptr += directory->size_of_files;
    }

    if (directory->number_of_child_directories == 0)
    {
        return;
    }

    for (int i = 0; i < directory->number_of_child_directories; i++)
    {
        struct directory *child_directory = directory->child_directories[i];

        check_directory_size(child_directory, sumptr);
    }
}