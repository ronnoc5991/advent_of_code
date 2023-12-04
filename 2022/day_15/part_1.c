#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100

typedef struct
{
    int x;
    int y;
} position;

typedef struct
{
    position position;
    position nearest_beacon;
    int range;
    int start_x;
    int end_x;
    int touches_target_index;
    int is_counted;
} sensor;

sensor parse_sensor(char *string, int target_row_index);
int insert_value(int **array, int *array_length, int value);
int find_value_in_array(int *array, int value, int start, int end);
int get_insertion_index(int *array, int array_length, int value);

int main(int argc, char **argv)
{
    char *filename = argv[1];
    int target_row_index = atoi(argv[2]);
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
        sensors[sensor_count - 1] = parse_sensor(current_line, target_row_index);
    }

    free(current_line);

    int non_beacon_count = 0;

    for (int i = 0; i < sensor_count; i++)
    {
        sensor *current = &sensors[i];

        if (!current->touches_target_index)
            continue;

        if (current->is_counted)
            continue;

        // trying to detmine if this sensor's range (or how much) should be counted

        for (int j = i + 1; j < sensor_count; j++)
        {
            // compare current to all future sensors
            sensor *next = &sensors[j];

            // if this sensor does not impact the target index, do not worry about its range
            if (!next->touches_target_index)
                continue;

            // complete overlap
            if (current->start_x >= next->start_x && current->start_x <= next->end_x && current->end_x >= next->start_x && current->end_x <= next->end_x)
            {
                // current is contained in next
                current->is_counted = 1;
                break;
            }
            else if (next->start_x >= current->start_x && next->start_x <= current->end_x && next->end_x >= current->start_x && next->end_x <= current->end_x)
            {
                // next is contained in current
                next->is_counted = 1;
            }
            else if (current->start_x >= next->start_x && current->start_x <= next->end_x)
            {
                // current start is in next, current end is not in next
                current->start_x = next->end_x + 1;
            }
            else if (current->end_x >= next->start_x && current->end_x <= next->end_x)
            {
                // current end is in next, current start is not in next
                current->end_x = next->start_x - 1;
            }

            // if it is not complete overlap, and there is no partial overlap -> continue
        }

        if (!current->is_counted)
        {
            non_beacon_count += current->end_x - current->start_x + 1;
            current->is_counted = 1;
        }
    }

    int found_values_count = 0;
    int *found_values = malloc(sizeof(int) * found_values_count);

    for (int i = 0; i < sensor_count; i++)
    {
        sensor *current = &sensors[i];

        if (current->position.y == target_row_index)
        {
            int is_duplicate_value = 0;

            for (int j = 0; j < found_values_count; j++)
            {
                if (found_values[j] == current->position.x)
                    is_duplicate_value = 1;
            }

            if (is_duplicate_value)
                break;

            found_values_count++;
            found_values = realloc(found_values, sizeof(int) * found_values_count);
            found_values[found_values_count - 1] = current->position.x;
        }

        if (current->nearest_beacon.y == target_row_index)
        {
            int is_duplicate_value = 0;

            for (int j = 0; j < found_values_count; j++)
            {
                if (found_values[j] == current->nearest_beacon.x)
                    is_duplicate_value = 1;
            }

            if (is_duplicate_value)
                break;

            found_values_count++;
            found_values = realloc(found_values, sizeof(int) * found_values_count);
            found_values[found_values_count - 1] = current->nearest_beacon.x;
        }
    }

    non_beacon_count -= found_values_count;

    printf("%i\n", non_beacon_count);

    free(sensors);

    return 0;
}

sensor parse_sensor(char *string, int target_row_index)
{
    char *x = malloc(sizeof(char));
    char *y = malloc(sizeof(char));

    char *beacon_x = malloc(sizeof(char));
    char *beacon_y = malloc(sizeof(char));

    int has_sensor_x = 0;
    int has_sensor_y = 0;

    while (*string != '\0')
    {
        if (*string == 'x')
        {
            string += 2; // skip to the number
            int length_of_x = 1;
            char **new_x = has_sensor_x ? &beacon_x : &x;

            do
            {
                length_of_x++;
                *new_x = realloc(*new_x, sizeof(char) * length_of_x);
                (*new_x)[length_of_x - 2] = *string;
                string++;
            } while (*string != ',');

            (*new_x)[length_of_x - 1] = '\0';
            if (!has_sensor_x)
                has_sensor_x = 1;
        }

        if (*string == 'y')
        {
            string += 2; // skip to the number
            int length_of_y = 1;
            char **new_y = has_sensor_y ? &beacon_y : &y;

            do
            {
                length_of_y++;
                *new_y = realloc(*new_y, sizeof(char) * length_of_y);
                (*new_y)[length_of_y - 2] = *string;
                string++;
            } while (*string != ':' && *string != '\0');

            (*new_y)[length_of_y - 1] = '\0';
            if (!has_sensor_y)
                has_sensor_y = 1;
        }

        if (*string != '\0')
            string++;
    }

    sensor new_sensor;
    new_sensor.position.x = atoi(x);
    new_sensor.position.y = atoi(y);
    new_sensor.nearest_beacon.x = atoi(beacon_x);
    new_sensor.nearest_beacon.y = atoi(beacon_y);
    new_sensor.range = abs(new_sensor.position.x - new_sensor.nearest_beacon.x) + abs(new_sensor.position.y - new_sensor.nearest_beacon.y);
    int start_y = new_sensor.position.y - new_sensor.range;
    int end_y = new_sensor.position.y + new_sensor.range;
    new_sensor.touches_target_index = start_y <= target_row_index && end_y >= target_row_index;
    new_sensor.is_counted = 0;

    if (new_sensor.touches_target_index)
    {
        int start = new_sensor.position.x - new_sensor.range;
        int end = new_sensor.position.x + new_sensor.range;

        int start_distance = abs(start - new_sensor.position.x) + abs(target_row_index - new_sensor.position.y);

        while (start_distance > new_sensor.range)
        {
            start++;
            start_distance = abs(start - new_sensor.position.x) + abs(target_row_index - new_sensor.position.y);
        }

        new_sensor.start_x = start;

        int end_distance = abs(end - new_sensor.position.x) + abs(target_row_index - new_sensor.position.y);

        while (end_distance > new_sensor.range)
        {
            end--;
            end_distance = abs(end - new_sensor.position.x) + abs(target_row_index - new_sensor.position.y);
        }

        new_sensor.end_x = end;
    }

    free(x);
    free(y);
    free(beacon_x);
    free(beacon_y);

    return new_sensor;
}