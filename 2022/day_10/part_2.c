#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INSTRUCTION_LENGTH 10
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 6

typedef struct
{
    int x;
    int y;
} pixel;

void draw_pixel(pixel *current_pixel, int sprite_x, char screen[SCREEN_WIDTH][SCREEN_HEIGHT]);
void move_to_next_pixel(pixel *current_pixel);

int main(void)
{
    char *filename = "input.txt";
    FILE *fp = fopen(filename, "r");

    int sprite_x = 1;
    char screen[SCREEN_WIDTH][SCREEN_HEIGHT];
    pixel current_pixel = {.x = 0, .y = 0};

    char current_instruction[MAX_INSTRUCTION_LENGTH];

    // per cycle
    // keep track of the current pixel
    // see if the sprite is in the current pixel
    // draw the current pixel

    // per cycle
    // get and decode an instruction if possible
    // draw the current pixel

    while (fgets(current_instruction, MAX_INSTRUCTION_LENGTH, fp) != NULL)
    {
        int cycles_to_wait = 0;
        int value_to_add = 0;

        if (current_instruction[strlen(current_instruction) - 1] == '\n')
        {
            current_instruction[strlen(current_instruction) - 1] = 0;
        }

        char *op_code = strtok(current_instruction, " ");

        if (strcmp(op_code, "noop") == 0)
        {
            cycles_to_wait = 1;
        }
        else
        {
            cycles_to_wait = 2;
            value_to_add = atoi(strtok(NULL, " "));
        }

        for (int i = 0; i < cycles_to_wait; i++)
        {
            draw_pixel(&current_pixel, sprite_x, screen);
            move_to_next_pixel(&current_pixel);
        }

        sprite_x += value_to_add;
    }

    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (int j = 0; j < SCREEN_WIDTH; j++)
        {
            printf("%c", screen[j][i]);
        }

        printf("\n");
    }

    return 0;
}

void draw_pixel(pixel *current_pixel, int sprite_x, char screen[SCREEN_WIDTH][SCREEN_HEIGHT])
{
    int is_sprite_visible = abs(current_pixel->x - sprite_x) < 2 ? 1 : 0;
    printf("%i - %i - %i\n", sprite_x, current_pixel->x, is_sprite_visible);
    screen[current_pixel->x][current_pixel->y] = is_sprite_visible ? '#' : '.';
}

void move_to_next_pixel(pixel *current_pixel)
{
    int new_x = current_pixel->x + 1;
    int new_y = current_pixel->y;

    if (new_x == SCREEN_WIDTH)
    {
        new_x = 0;
        new_y += 1;
    }

    current_pixel->x = new_x;
    current_pixel->y = new_y;
}