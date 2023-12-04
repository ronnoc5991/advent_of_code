
#include <stdio.h>

#define MAXDIGITS 100 /* maximum length bignum */

#define PLUS 1   /* positive sign bit */
#define MINUS -1 /* negative sign bit */

typedef struct
{
    char digits[MAXDIGITS]; /* represent the number */
    int signbit;            /* 1 if positive, -1 if negative */
    int lastdigit;          /* index of high-order digit */
} bignum;

print_bignum(bignum *n)
{
    int i;

    if (n->signbit == MINUS)
        printf("- ");
    for (i = n->lastdigit; i >= 0; i--)
        printf("%c", '0' + n->digits[i]);

    printf("\n");
}

int_to_bignum(int s, bignum *n)
{
    int i; /* counter */
    int t; /* int to work with */

    if (s >= 0)
        n->signbit = PLUS;
    else
        n->signbit = MINUS;

    for (i = 0; i < MAXDIGITS; i++)
        n->digits[i] = (char)0;

    n->lastdigit = -1;

    t = abs(s);

    while (t > 0)
    {
        n->lastdigit++;
        n->digits[n->lastdigit] = (t % 10);
        t = t / 10;
    }

    if (s == 0)
        n->lastdigit = 0;
}

initialize_bignum(bignum *n)
{
    int_to_bignum(0, n);
}

int max(int a, int b)
{
    if (a > b)
        return (a);
    else
        return (b);
}

/*	c = a +-/* b;	*/

add_bignum(bignum *a, bignum *b, bignum *c)
{
    int carry; /* carry digit */
    int i;     /* counter */

    initialize_bignum(c);

    if (a->signbit == b->signbit)
        c->signbit = a->signbit;
    else
    {
        if (a->signbit == MINUS)
        {
            a->signbit = PLUS;
            subtract_bignum(b, a, c);
            a->signbit = MINUS;
        }
        else
        {
            b->signbit = PLUS;
            subtract_bignum(a, b, c);
            b->signbit = MINUS;
        }
        return;
    }

    c->lastdigit = max(a->lastdigit, b->lastdigit) + 1;
    carry = 0;

    for (i = 0; i <= (c->lastdigit); i++)
    {
        c->digits[i] = (char)(carry + a->digits[i] + b->digits[i]) % 10;
        carry = (carry + a->digits[i] + b->digits[i]) / 10;
    }

    zero_justify(c);
}

zero_justify(bignum *n)
{
    while ((n->lastdigit > 0) && (n->digits[n->lastdigit] == 0))
        n->lastdigit--;

    if ((n->lastdigit == 0) && (n->digits[0] == 0))
        n->signbit = PLUS; /* hack to avoid -0 */
}

digit_shift(bignum *n, int d) /* multiply n by 10^d */
{
    int i; /* counter */

    if ((n->lastdigit == 0) && (n->digits[0] == 0))
        return;

    for (i = n->lastdigit; i >= 0; i--)
        n->digits[i + d] = n->digits[i];

    for (i = 0; i < d; i++)
        n->digits[i] = 0;

    n->lastdigit = n->lastdigit + d;
}

multiply_bignum(bignum *a, bignum *b, bignum *c)
{
    bignum row; /* represent shifted row */
    bignum tmp; /* placeholder bignum */
    int i, j;   /* counters */

    initialize_bignum(c);

    row = *a;

    for (i = 0; i <= b->lastdigit; i++)
    {
        for (j = 1; j <= b->digits[i]; j++)
        {
            add_bignum(c, &row, &tmp);
            *c = tmp;
        }
        digit_shift(&row, 1);
    }

    c->signbit = a->signbit * b->signbit;

    zero_justify(c);
}

#include <stdio.h>

#define NUMBER_OF_MONKEYS 8
#define NUMBER_OF_ITEMS 36
#define NUMBER_OF_ROUNDS 1000

typedef struct
{
    int index;
    int inspection_count;
    int number_of_items;
    unsigned long long items[NUMBER_OF_ITEMS];
    int throws[NUMBER_OF_ITEMS];
} monkey;

// we cannot store the items as longs/ints anymore
// can we store them as arrays of ints?
// the items array can be an array of pointers
// each pointer will point to an array of ints

void initialize_monkeys(monkey monkeys[NUMBER_OF_MONKEYS]);
void inspect_items(monkey *active_monkey);
unsigned long long inspect_item(unsigned long long item, int monkey_index);
unsigned long long inspect_monkey_zero(unsigned long long old_item);
unsigned long long inspect_monkey_one(unsigned long long old_item);
unsigned long long inspect_monkey_two(unsigned long long old_item);
unsigned long long inspect_monkey_three(unsigned long long old_item);
unsigned long long inspect_monkey_four(unsigned long long old_item);
unsigned long long inspect_monkey_five(unsigned long long old_item);
unsigned long long inspect_monkey_six(unsigned long long old_item);
unsigned long long inspect_monkey_seven(unsigned long long old_item);
void test_items(monkey *active_monkey);
int test_item(unsigned long long item, int monkey_index);
int test_monkey_zero(unsigned long long item);
int test_monkey_one(unsigned long long item);
int test_monkey_two(unsigned long long item);
int test_monkey_three(unsigned long long item);
int test_monkey_four(unsigned long long item);
int test_monkey_five(unsigned long long item);
int test_monkey_six(unsigned long long item);
int test_monkey_seven(unsigned long long item);
void throw_items(monkey monkeys[NUMBER_OF_MONKEYS], monkey *current_monkey);
void throw_item(unsigned long long item, monkey *from_monkey, monkey *to_monkey);

unsigned long long monkeys_items[NUMBER_OF_MONKEYS][NUMBER_OF_ITEMS] = {
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
            unsigned long long item = monkeys_items[i][j];
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
        unsigned long long current_item = active_monkey->items[i];
        if (current_item < 0)
        {
            printf("negative item: -> %i - %i : %lli\n", item_count, i, current_item);
            continue;
        }

        unsigned long long new_item = inspect_item(current_item, active_monkey->index);
        active_monkey->items[i] = new_item;
        active_monkey->inspection_count++;
        if (new_item < 0)
        {
            printf("Created negative: (before: %lli - after: %lli)\n", current_item, new_item);
        }
    }
}

unsigned long long inspect_item(unsigned long long item, int monkey_index)
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

unsigned long long inspect_monkey_zero(unsigned long long old_item)
{
    return old_item * 5;
}

unsigned long long inspect_monkey_one(unsigned long long old_item)
{
    return old_item + 1;
}

unsigned long long inspect_monkey_two(unsigned long long old_item)
{
    return old_item + 8;
}

unsigned long long inspect_monkey_three(unsigned long long old_item)
{
    return old_item + 4;
}

unsigned long long inspect_monkey_four(unsigned long long old_item)
{
    return old_item + 7;
}

unsigned long long inspect_monkey_five(unsigned long long old_item)
{
    return old_item + 2;
}

unsigned long long inspect_monkey_six(unsigned long long old_item)
{
    return old_item * 11;
}

unsigned long long inspect_monkey_seven(unsigned long long old_item)
{
    return old_item * old_item;
}

void test_items(monkey *active_monkey)
{
    int item_count = active_monkey->number_of_items;

    for (int i = 0; i < item_count; i++)
    {
        unsigned long long current_item = active_monkey->items[i];
        int to_monkey_index = test_item(current_item, active_monkey->index);
        active_monkey->throws[i] = to_monkey_index;
    }
}

int test_item(unsigned long long item, int monkey_index)
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

int test_monkey_zero(unsigned long long item)
{
    if (item % 7 == 0)
    {
        return 6;
    }

    return 7;
}

int test_monkey_one(unsigned long long item)
{
    if (item % 17 == 0)
    {
        return 0;
    }

    return 6;
}

int test_monkey_two(unsigned long long item)
{
    if (item % 11 == 0)
    {
        return 5;
    }

    return 3;
}

int test_monkey_three(unsigned long long item)
{
    if (item % 13 == 0)
    {
        return 0;
    }

    return 1;
}

int test_monkey_four(unsigned long long item)
{
    if (item % 19 == 0)
    {
        return 5;
    }

    return 2;
}

int test_monkey_five(unsigned long long item)
{
    if (item % 2 == 0)
    {
        return 1;
    }

    return 3;
}

int test_monkey_six(unsigned long long item)
{
    if (item % 5 == 0)
    {
        return 7;
    }

    return 4;
}

int test_monkey_seven(unsigned long long item)
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

void throw_item(unsigned long long item, monkey *from_monkey, monkey *to_monkey)
{
    if (item < 0)
    {
        printf("throwing negative number\n");
    }
    to_monkey->items[to_monkey->number_of_items] = item;
    from_monkey->number_of_items--;
    to_monkey->number_of_items++;
}