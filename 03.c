#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 140

char *schematic;

#define T(x, y)     schematic[(SIZE * (y)) + (x)] // Helper for easier access
#define MIN(a, b)   ((a) < (b)) ? (a) : (b)
#define MAX(a, b)   ((a) > (b)) ? (a) : (b)

typedef struct {
    int x;
    int y;
    int value;
} Gear;

int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

int is_part_number(int start_x, int start_y, int length)
{
    // Left and right
    if (start_x > 0 && (T(start_x - 1, start_y) != '.'))
        return 1;
    if (start_x + length < SIZE && T(start_x + length, start_y) != '.')
        return 1;

    // Top and bottom
    int from = MAX(start_x - 1, 0);
    int to = MIN(start_x + length + 1, SIZE);
    for (int x = from; x < to; ++x) {
        if (start_y > 0 && T(x, start_y - 1) != '.')
            return 1;
        if (start_y < SIZE - 1 && T(x, start_y + 1) != '.')
            return 1;
    }

    return 0;
}

int find_gear(Gear *gear, int start_x, int start_y, int length)
{
    // Left and right
    if (start_x > 0 && (T(start_x - 1, start_y) == '*')) {
        gear->x = start_x - 1;
        gear->y = start_y;
        return 1;
    }
    if (start_x + length < SIZE && T(start_x + length, start_y) == '*') {
        gear->x = start_x + length;
        gear->y = start_y;
        return 1;
    }

    // Top and bottom
    int from = MAX(start_x - 1, 0);
    int to = MIN(start_x + length + 1, SIZE);
    for (int x = from; x < to; ++x) {
        if (start_y > 0 && T(x, start_y - 1) == '*') {
            gear->x = x;            gear->y = start_y;
            gear->y = start_y - 1;
            return 1;
        }
        if (start_y < SIZE - 1 && T(x, start_y + 1) == '*') {
            gear->x = x;            gear->y = start_y;
            gear->y = start_y + 1;
            return 1;
        }
    }

    return 0;
}

int part1()
{
    char c;
    int start_x, start_y, length;

    int num = 0;
    int sum = 0;
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            c = T(x, y);
            if (is_digit(c)) {
                if (!num) {
                    num = c - '0';
                    start_x = x;
                    start_y = y;
                    length = 1;
                } else {
                    num = 10 * num + (c - '0');
                    ++length;
                }
                if (x == SIZE - 1) {
                    if (is_part_number(start_x, start_y, length))
                        sum += num;
                    num = 0;
                }
            } else {
                if (num) {
                    if (is_part_number(start_x, start_y, length))
                        sum += num;
                    num = 0;
                }
            }
        }

    }

    return sum;
}

int part2()
{
    char c;
    int start_x, start_y, length;

    Gear gear = {0};
    int gears_count = 0;
    Gear *gears = malloc(1024 * sizeof(Gear)); // Should be enough :)

    int num = 0;
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            c = T(x, y);
            if (is_digit(c)) {
                if (!num) {
                    num = c - '0';
                    start_x = x;
                    start_y = y;
                    length = 1;
                } else {
                    num = 10 * num + (c - '0');
                    ++length;
                }

                if (x == SIZE - 1) {
                    if (find_gear(&gear, start_x, start_y, length)) {
                        gear.value = num;
                        gears[gears_count++] = gear;
                    }
                    num = 0;
                }
            } else {
                if (num) {
                    if (find_gear(&gear, start_x, start_y, length)) {
                        gear.value = num;
                        gears[gears_count++] = gear;
                    }
                    num = 0;
                }
            }
        }
    }

    int sum = 0;
    for (int i = 0; i < gears_count - 1; ++i) {
        for (int j = i + 1; j < gears_count; ++j) {
            Gear *a = gears + i;
            Gear *b = gears + j;
            if (a->x == b->x && a->y == b->y)
                sum += a->value * b->value;
        }
    }

    free(gears);

    return sum;
}

int main()
{
    schematic = (char *) malloc(SIZE * SIZE);

    char line[256];
    char *filename = "input.txt";
    FILE* file;

    int row = 0, col;
    file = fopen(filename, "r");
    while (fgets(line, sizeof(line), file)) {
        for (col = 0; col < SIZE; ++col)
            T(col, row) = line[col];
        row++;
    }
    fclose(file);

    printf("%d\n", part1());
    printf("%d\n", part2());

    free(schematic);
    return 0;
}
