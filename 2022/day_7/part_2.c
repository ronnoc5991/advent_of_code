#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 30
#define MAX_NAME_LENGTH 20
#define MAX_DIRECTORY_SIZE 100000
#define TOTAL_DISK_SPACE 70000000
#define UNUSED_SPACE_NEEDED 30000000

struct directory *create_new_directory(char *name, struct directory *parent_directory);
void save_directory_name(char *new_directory_name, char *name);
void collect_file_sizes(struct directory *current_directory, int *file_sizes, int *files_size_index);
void sort_int_array(int *array, int array_length);

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

    int directory_count = 0;

    struct directory *pwdptr;
    struct directory *home_directory = create_new_directory("/", NULL);
    directory_count++;

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
            directory_count++;

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

    int total_used_space = home_directory->size_of_files;
    int current_unused_space = TOTAL_DISK_SPACE - total_used_space;
    int space_needed = UNUSED_SPACE_NEEDED - current_unused_space;

    int file_sizes[directory_count];

    for (int i = 0; i < directory_count; i++)
    {
        file_sizes[i] = 0;
    }

    int file_sizes_index = 0;

    collect_file_sizes(home_directory, file_sizes, &file_sizes_index);

    sort_int_array(file_sizes, directory_count);

    printf("unused: %i - space needed: %i - ", current_unused_space, space_needed);

    for (int i = 0; i < directory_count; i++)
    {
        int current_size = file_sizes[i];
        if (current_size > space_needed)
        {
            printf("%i", file_sizes[i]);
            break;
        }
    }

    // loop over that array
    // find the first value that satisfies our problem

    // need to now sort that array

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

void collect_file_sizes(struct directory *current_directory, int *file_sizes, int *files_size_index)
{
    file_sizes[*files_size_index] = current_directory->size_of_files;
    *files_size_index += 1;

    // call this function for every child directory

    if (current_directory->number_of_child_directories == 0)
    {
        return;
    }

    for (int i = 0; i < current_directory->number_of_child_directories; i++)
    {
        collect_file_sizes(current_directory->child_directories[i], file_sizes, files_size_index);
    }
}

void sort_int_array(int *array, int array_length)
{
    // keep a has_made_change bool
    // if that is false at the end of the loop, we can break

    // i want to execute this for loop until we go through it once cleanly
    int has_made_change;
    do
    {
        has_made_change = 0;
        for (int i = 0; i < array_length - 1; i++)
        {
            int first_int = array[i];
            int second_int = array[i + 1];

            if (first_int > second_int)
            {
                array[i] = second_int;
                array[i + 1] = first_int;
                has_made_change = 1;
            }
        }
    } while (has_made_change);
}