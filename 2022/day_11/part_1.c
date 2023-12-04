#include <stdio.h>

#define NUMBER_OF_MONKEYS 8
#define NUMBER_OF_ITEMS 36
#define NUMBER_OF_ROUNDS 10000

typedef struct
{
    int index;
    int inspection_count;
    int number_of_items;
    long items[NUMBER_OF_ITEMS];
    int throws[NUMBER_OF_ITEMS];
} monkey;

void initialize_monkeys(monkey monkeys[NUMBER_OF_MONKEYS]);
void inspect_items(monkey *active_monkey);
long inspect_item(long item, int monkey_index);
long inspect_monkey_zero(long old_item);
long inspect_monkey_one(long old_item);
long inspect_monkey_two(long old_item);
long inspect_monkey_three(long old_item);
long inspect_monkey_four(long old_item);
long inspect_monkey_five(long old_item);
long inspect_monkey_six(long old_item);
long inspect_monkey_seven(long old_item);
void test_items(monkey *active_monkey);
int test_item(long item, int monkey_index);
int test_monkey_zero(long item);
int test_monkey_one(long item);
int test_monkey_two(long item);
int test_monkey_three(long item);
int test_monkey_four(long item);
int test_monkey_five(long item);
int test_monkey_six(long item);
int test_monkey_seven(long item);
void throw_items(monkey monkeys[NUMBER_OF_MONKEYS], monkey *current_monkey);
void throw_item(long item, monkey *from_monkey, monkey *to_monkey);

long monkeys_items[NUMBER_OF_MONKEYS][NUMBER_OF_ITEMS] = {
    {89, 84, 88, 78, 70, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0},
    {76, 62, 61, 54, 69, 60, 85, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0},
    {83, 89, 53, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0},
    {95, 94, 85, 57, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0},
    {82, 98, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0},
    {69, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0},
    {82, 70, 58, 87, 59, 99, 92, 65, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0},
    {91, 53, 96, 98, 68, 82, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0},
};

int main(void)
{
    monkey monkeys[NUMBER_OF_MONKEYS];

    initialize_monkeys(monkeys);

    for (int i = 0; i < NUMBER_OF_ROUNDS; i++)
    {
        for (int j = 0; j < NUMBER_OF_MONKEYS; j++)
        {
            if (monkeys[j].number_of_items == 0)
            {
                continue;
            }
            inspect_items(&monkeys[j]);
            test_items(&monkeys[j]);
            throw_items(monkeys, &monkeys[j]);
        }
    }

    for (int i = 0; i < NUMBER_OF_MONKEYS; i++)
    {
        printf("Monkey %i: %i\n", i, monkeys[i].inspection_count);
    }

    return 0;
}

void initialize_monkeys(monkey monkeys[NUMBER_OF_MONKEYS])
{
    for (int i = 0; i < NUMBER_OF_MONKEYS; i++)
    {
        monkey new_monkey;

        int item_count = 0;

        for (int j = 0; j < NUMBER_OF_ITEMS; j++)
        {
            long item = monkeys_items[i][j];
            if (item > 0)
            {
                item_count++;
            }
            new_monkey.items[j] = item;
            new_monkey.throws[j] = -1;
        }

        new_monkey.number_of_items = item_count;
        new_monkey.inspection_count = 0;
        new_monkey.index = i;

        monkeys[i] = new_monkey;
    }
}

void inspect_items(monkey *active_monkey)
{
    int item_count = active_monkey->number_of_items;
    for (int i = 0; i < item_count; i++)
    {
        long current_item = active_monkey->items[i];
        if (current_item < 0)
        {
            printf("negative item: -> %i - %i : %li\n", item_count, i, current_item);
            continue;
        }

        long new_item = inspect_item(current_item, active_monkey->index);
        active_monkey->items[i] = new_item;
        active_monkey->inspection_count++;
        if (new_item < 0)
        {
            printf("Created negative: (before: %li - after: %li)\n", current_item, new_item);
        }
    }
}

long inspect_item(long item, int monkey_index)
{
    switch (monkey_index)
    {
    case 0:
        return inspect_monkey_zero(item);
    case 1:
        return inspect_monkey_one(item);
    case 2:
        return inspect_monkey_two(item);
    case 3:
        return inspect_monkey_three(item);
    case 4:
        return inspect_monkey_four(item);
    case 5:
        return inspect_monkey_five(item);
    case 6:
        return inspect_monkey_six(item);
    case 7:
        return inspect_monkey_seven(item);
    default:
        return 0;
    }
}

long inspect_monkey_zero(long old_item)
{
    return old_item * 5;
}

long inspect_monkey_one(long old_item)
{
    return old_item + 1;
}

long inspect_monkey_two(long old_item)
{
    return old_item + 8;
}

long inspect_monkey_three(long old_item)
{
    return old_item + 4;
}

long inspect_monkey_four(long old_item)
{
    return old_item + 7;
}

long inspect_monkey_five(long old_item)
{
    return old_item + 2;
}

long inspect_monkey_six(long old_item)
{
    return old_item * 11;
}

long inspect_monkey_seven(long old_item)
{
    return old_item * old_item;
}

void test_items(monkey *active_monkey)
{
    int item_count = active_monkey->number_of_items;

    for (int i = 0; i < item_count; i++)
    {
        long current_item = active_monkey->items[i];
        int to_monkey_index = test_item(current_item, active_monkey->index);
        active_monkey->throws[i] = to_monkey_index;
    }
}

int test_item(long item, int monkey_index)
{
    switch (monkey_index)
    {
    case 0:
        return test_monkey_zero(item);
    case 1:
        return test_monkey_one(item);
    case 2:
        return test_monkey_two(item);
    case 3:
        return test_monkey_three(item);
    case 4:
        return test_monkey_four(item);
    case 5:
        return test_monkey_five(item);
    case 6:
        return test_monkey_six(item);
    case 7:
        return test_monkey_seven(item);
    default:
        return 0;
    }
}

int test_monkey_zero(long item)
{
    if (item % 7 == 0)
    {
        return 6;
    }

    return 7;
}

int test_monkey_one(long item)
{
    if (item % 17 == 0)
    {
        return 0;
    }

    return 6;
}

int test_monkey_two(long item)
{
    if (item % 11 == 0)
    {
        return 5;
    }

    return 3;
}

int test_monkey_three(long item)
{
    if (item % 13 == 0)
    {
        return 0;
    }

    return 1;
}

int test_monkey_four(long item)
{
    if (item % 19 == 0)
    {
        return 5;
    }

    return 2;
}

int test_monkey_five(long item)
{
    if (item % 2 == 0)
    {
        return 1;
    }

    return 3;
}

int test_monkey_six(long item)
{
    if (item % 5 == 0)
    {
        return 7;
    }

    return 4;
}

int test_monkey_seven(long item)
{
    if (item % 3 == 0)
    {
        return 4;
    }

    return 2;
}

void throw_items(monkey monkeys[NUMBER_OF_MONKEYS], monkey *current_monkey)
{
    int item_count = current_monkey->number_of_items;

    for (int i = 0; i < item_count; i++)
    {
        if (current_monkey->throws[i] < 0 || current_monkey->throws[i] >= NUMBER_OF_MONKEYS)
        {
            printf("threw to a non existing monkey");
            return;
        }

        throw_item(current_monkey->items[i], current_monkey, &monkeys[current_monkey->throws[i]]);

        current_monkey->items[i] = -1;
        current_monkey->throws[i] = -1;
    }
}

void throw_item(long item, monkey *from_monkey, monkey *to_monkey)
{
    if (item < 0)
    {
        printf("throwing negative number\n");
    }
    to_monkey->items[to_monkey->number_of_items] = item;
    from_monkey->number_of_items--;
    to_monkey->number_of_items++;
}