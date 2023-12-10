#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT  140
#define WIDTH   140

typedef enum {
    GROUND      = '.',
    VERTICAL    = '|',
    HORIZONTAL  = '-',
    NORTH_EAST  = 'L',
    NORTH_WEST  = 'J',
    SOUTH_WEST  = '7',
    SOUTH_EAST  = 'F',
} Pipe;

typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;

static int start_x, start_y;
Pipe loop[HEIGHT * WIDTH];
bool is_part_of_loop[HEIGHT * WIDTH];

#define P(x, y) loop[((y) * WIDTH) + (x)]

void parse_map(char *filename);
int traverse();
int count_enclosed();
void visualize();

int main(int argc, char **argv)
{
    char *filename = argc > 1 ? argv[1] : "input.txt";

    parse_map(filename);

    int steps = traverse();
    int enclosed = count_enclosed();

    visualize();

    printf("%d\n", steps/2);
    printf("%d\n", enclosed);

    return 0;
}

void parse_map(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "ERROR: Unable to open file: '%s'.\n", filename);
        exit(1);
    }

    char c;
    int x = 0, y = 0;
    while ((c = fgetc(file)) != EOF) {
        x++;
        switch (c) {
            case 'S': {
                start_x = x;
                start_y = y;
                printf("Start: [%d, %d]\n", start_x, start_y);
                break;
            }
            case '\n': {
                x = 0;
                y++;
                break;
            }
            case GROUND    : P(x, y) = GROUND    ; break;
            case VERTICAL  : P(x, y) = VERTICAL  ; break;
            case HORIZONTAL: P(x, y) = HORIZONTAL; break;
            case NORTH_EAST: P(x, y) = NORTH_EAST; break;
            case NORTH_WEST: P(x, y) = NORTH_WEST; break;
            case SOUTH_WEST: P(x, y) = SOUTH_WEST; break;
            case SOUTH_EAST: P(x, y) = SOUTH_EAST; break;
            default:
                fprintf(stderr, "Unhandled tile '%c' at [%d, %d].\n", c, x, y);
                break;
        }
    }
}

int traverse()
{
    int steps = 0;

    int x = start_x;
    int y = start_y;
    Direction direction = EAST;

    for (;;) {
        steps++;
        is_part_of_loop[y*WIDTH+x] = true;
        switch (direction) {
            case NORTH:
                y--;
                if (x == start_x && y == start_y)
                    return steps;

                switch (P(x, y)) {
                    case VERTICAL:
                        break;
                    case SOUTH_WEST:
                        direction = WEST;
                        break;
                    case SOUTH_EAST:
                        direction = EAST;
                        break;
                    default:
                        fprintf(stderr, "ERROR: Invalid pipe config at [%d, %d].\n", x, y);
                        exit(1);
                }
                break;
            case EAST:
                x++;
                if (x == start_x && y == start_y)
                    return steps;

                switch (P(x, y)) {
                    case HORIZONTAL:
                        break;
                    case NORTH_WEST:
                        direction = NORTH;
                        break;
                    case SOUTH_WEST:
                        direction = SOUTH;
                        break;
                    default:
                        fprintf(stderr, "ERROR: Invalid pipe config at [%d, %d].\n", x, y);
                        exit(1);
                }
                break;
            case SOUTH:
                y++;
                if (x == start_x && y == start_y)
                    return steps;

                switch (P(x, y)) {
                    case VERTICAL:
                        break;
                    case NORTH_WEST:
                        direction = WEST;
                        break;
                    case NORTH_EAST:
                        direction = EAST;
                        break;
                    default:
                        fprintf(stderr, "ERROR: Invalid pipe config at [%d, %d].\n", x, y);
                        exit(1);
                }
                break;
            case WEST:
                x--;
                if (x == start_x && y == start_y)
                    return steps;

                switch (P(x, y)) {
                    case HORIZONTAL:
                        break;
                    case NORTH_EAST:
                        direction = NORTH;
                        break;
                    case SOUTH_EAST:
                        direction = SOUTH;
                        break;
                    default:
                        fprintf(stderr, "ERROR: Invalid pipe config at [%d, %d].\n", x, y);
                        exit(1);
                }
                break;
            default:
                fprintf(stderr, "ERROR: Invalid direction %d.\n", direction);
        }
    }
}

int count_enclosed()
{
    int enclosed = 0;
    bool inside = false;
    Pipe last = GROUND;
    for (int y = 0; y < HEIGHT; ++y) {
        inside = false;
        for (int x = 0; x < WIDTH; ++x) {
            // Determine inside based on number of crosses of the main loop,
            // where crossing is defined as one of these three constructions:
            //      1 '|'
            //      2 'L---7'
            //      3 'F---J'
            if (is_part_of_loop[y * WIDTH + x] ) {
                Pipe p = P(x, y);
                if (p == VERTICAL
                        || (p == NORTH_WEST && last == SOUTH_EAST)
                        || (p == SOUTH_WEST && last == NORTH_EAST)) {
                    inside = !inside;
                }
                if (p != HORIZONTAL)
                    last = p;
            } else if (inside) {
                enclosed++;
            }
        }
    }

    return enclosed;
}

void visualize()
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
             if (is_part_of_loop[y * WIDTH + x]) {
                 printf("%c", P(x, y));
             } else {
                 printf("%c", '.');
             }
        }
        printf("\n");
    }
}
