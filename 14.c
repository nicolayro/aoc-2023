#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 128 * 128
#define BILLION  1000000000

int N, M;
char *platform;

int cycle_start, cycle_length;
char *tortoise;
char *hare;

#define P(x, y) platform[((y) * N) + (x)]
#define T(x, y) tortoise[((y) * N) + (x)]
#define H(x, y) hare[((y) * N) + (x)]

void parse_platform(char *filename);
void tilt_platform_north(char *arr);
void cycle_n_times(char *arr, int n);
void detect_cycle();
int calculate_load();

int main(int argc, char **argv)
{
    int load;
    char *filename = argc > 1 ? argv[1] : "input.txt";

    platform = malloc(MAX_SIZE * sizeof(char));
    tortoise = malloc(MAX_SIZE * sizeof(char));
    hare     = malloc(MAX_SIZE * sizeof(char));

    // Part 1
    parse_platform(filename);
    tilt_platform_north(platform);
    load = calculate_load();
    printf("%d\n", load);

    // Part 2
    parse_platform(filename);
    detect_cycle();
    cycle_n_times(platform, BILLION);
    load = calculate_load();
    printf("%d\n", load);

    free(platform);
    free(tortoise);
    free(hare);

    return 0;
}


void parse_platform(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "ERROR: Unable to open file '%s'.\n", filename);
        exit(1);
    }

    int x, y = 0;
    char line[MAX_SIZE];
    while (fgets(line, MAX_SIZE, file)) {
        N = strlen(line) - 1;

        for (x = 0; x < N; ++x) {
            P(x, y) = line[x];
        }

        y++;
    }
    M = y;

    // Copy domains for part 2
    for (int y = 0; y < M; ++y) {
        for (int x = 0; x < N; ++x) {
            T(x, y) = P(x, y);
            H(x, y) = P(x, y);
        }
    }

    fclose(file);
}

#define A(x, y) arr[((y) * N) + (x)]

void tilt_platform_north(char *arr)
{
    for (int time_step = 0; time_step < N; ++time_step) {
        for (int y = 1; y < M; ++y) {
            for (int x = 0; x < N; ++x) {
                if (A(x, y) == 'O' && A(x, y-1) == '.') {
                    A(x, y-1) = 'O';
                    A(x, y)   = '.';
                }
            }
        }
    }
}

void tilt_platform_west(char *arr)
{
    for (int time_step = 0; time_step < N; ++time_step) {
        for (int y = 0; y < M; ++y) {
            for (int x = 1; x < N; ++x) {
                if (A(x, y) == 'O' && A(x-1, y) == '.') {
                    A(x-1, y) = 'O';
                    A(x, y)   = '.';
                }
            }
        }
    }
}

void tilt_platform_south(char *arr)
{
    for (int time_step = 0; time_step < M; ++time_step) {
        for (int y = M-2; y >= 0; --y) {
            for (int x = 0; x < N; ++x) {
                if (A(x, y) == 'O' && A(x, y+1) == '.') {
                    A(x, y+1) = 'O';
                    A(x, y)   = '.';
                }
            }
        }
    }
}

void tilt_platform_east(char *arr)
{
    for (int time_step = 0; time_step < N; ++time_step) {
        for (int y = 0; y < M; ++y) {
            for (int x = N - 2; x >= 0; --x) {
                if (A(x, y) == 'O' && A(x+1, y) == '.') {
                    A(x+1, y) = 'O';
                    A(x, y)   = '.';
                }
            }
        }
    }
}

void cycle(char *arr)
{
    tilt_platform_north(arr);
    tilt_platform_west(arr);
    tilt_platform_south(arr);
    tilt_platform_east(arr);
}

void cycle_n_times(char *arr, int n)
{
    int cycles;
    if (n > cycle_start && cycle_start && cycle_length) {
        cycles = cycle_start + (n - cycle_start) % cycle_length;
    } else {
        cycles = n;
    }

    for (int i = 0; i < cycles; ++i) {
        cycle(arr);
    }
}

int is_platform_equal(char *a, char *b)
{
    for (int y = 0; y < M; ++y) {
        for (int x = 1; x < N; ++x) {
            if (a[y*N+x] != b[y*N+x])
                return 0;
        }
    }
    return 1;
}

void detect_cycle()
{
    // Find cycle
    cycle_n_times(tortoise, 1);
    cycle_n_times(hare, 2);
    while (!is_platform_equal(tortoise, hare)) {
        cycle_n_times(tortoise, 1);
        cycle_n_times(hare, 2);
    }

    // Move tortoise to beginning
    for (int y = 0; y < M; ++y) {
        for (int x = 0; x < N; ++x) {
            T(x, y) = P(x, y);
        }
    }

    // Moving at the same speed, they should now meet at the cycle start
    int start = 0;
    while (!is_platform_equal(tortoise, hare)) {
        cycle_n_times(tortoise, 1);
        cycle_n_times(hare, 1);
        start++;
    }

    // Find length of cycle
    int length = 1;
    cycle_n_times(hare, 1);
    while (!is_platform_equal(tortoise, hare)) {
        cycle_n_times(hare, 1);
        length++;
    }

    cycle_start = start;
    cycle_length = length;
}

int calculate_load()
{
    int load = 0;
    for (int y = 0; y < M; ++y) {
        for (int x = 0; x < N; ++x) {
            if (P(x, y) == 'O') {
                load += (M-y);
            }
        }
    }
    return load;
}
