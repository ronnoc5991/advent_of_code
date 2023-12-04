#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100

// Now, for the space from 0, 0 to 4000000, 4000000
// need to find the space that is not occupied

//         0      4000000
//         . . . . . . . .
//         . . . . . . . .
//         . . . . . . . .
// 4000000 . . . . . . . .

typedef struct
{
    int x;
    int y;
} position;

typedef struct
{
    position position;
    int range;
    int min_x;
    int max_x;
    int min_y;
    int max_y;
} sensor;

sensor parse_sensor(char *string);

int main(int argc, char **argv)
{
    char *filename = argv[1];
    int max_value = atoi(argv[2]);
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file: %s\n", filename);
        return 1;
    }

    int sensor_count = 0;
    sensor *sensors = malloc(sizeof(sensor) * sensor_count);

    char *current_line = malloc(sizeof(char) * MAX_LINE_LENGTH);
    char *line_delimiter = ":";

    while (fgets(current_line, MAX_LINE_LENGTH, fp) != NULL)
    {
        if (current_line[strlen(current_line) - 1] == '\n')
            current_line[strlen(current_line) - 1] = 0;

        sensor_count++;
        sensors = realloc(sensors, sizeof(sensor) * sensor_count);
        sensors[sensor_count - 1] = parse_sensor(current_line);
    }

    free(current_line);

    for (int i = 0; i < sensor_count; i++)
    {
        sensor *current = &sensors[i];

        if (current->max_y < 0 || current->min_x > max_value || current->min_y > max_value || current->max_x < 0)
            continue; // this sensor does not impact our area of interest
    }

    free(sensors);

    return 0;
}

sensor parse_sensor(char *string)
{
    char *x = malloc(sizeof(char));
    char *y = malloc(sizeof(char));
    char *beacon_x = malloc(sizeof(char));
    char *beacon_y = malloc(sizeof(char));

    int has_sensor_x = 0;
    int has_sensor_y = 0;

    while (*string != '\0')
    {
        int is_x = *string == 'x';
        int is_y = *string == 'y';

        if (is_x || is_y)
        {
            string += 2; // skip to the number
            int length = 1;
            char **new_value;

            if (is_x)
                new_value = has_sensor_x ? &beacon_x : &x;
            else
                new_value = has_sensor_y ? &beacon_y : &y;

            do
            {
                length++;
                *new_value = realloc(*new_value, sizeof(char) * length);
                (*new_value)[length - 2] = *string;
                string++;
            } while (*string != ',');

            (*new_value)[length - 1] = '\0';

            if (is_x && !has_sensor_x)
                has_sensor_x = 1;
            else if (is_y && !has_sensor_y)
                has_sensor_y = 1;
        }

        if (*string != '\0')
            string++;
    }

    sensor new_sensor;
    new_sensor.position.x = atoi(x);
    new_sensor.position.y = atoi(y);
    new_sensor.range = abs(new_sensor.position.x - atoi(beacon_x)) + abs(new_sensor.position.y - atoi(beacon_y));
    new_sensor.min_x = new_sensor.position.x - new_sensor.range;
    new_sensor.max_x = new_sensor.position.x + new_sensor.range;
    new_sensor.min_y = new_sensor.position.y - new_sensor.range;
    new_sensor.max_y = new_sensor.position.y + new_sensor.range;

    free(x);
    free(y);
    free(beacon_x);
    free(beacon_y);

    return new_sensor;
}