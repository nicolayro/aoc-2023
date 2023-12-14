#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WIDTH   32
#define MAX_HEIGHT  32

static int N, M;
uint8_t pattern[MAX_WIDTH * MAX_HEIGHT];

int find_symmetry(int flip, int prev);

int main(int argc, char **argv)
{
    int sum1 = 0;
    int sum2 = 0;

    char *filename = argc > 1 ? argv[1] : "input.txt";

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "ERROR: Unable to open file '%s'.\n", filename);
        exit(1);
    }

    int x, y = 0;
    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n') {
            M = y;
            int solution = find_symmetry(0, 0);
            sum1 += solution;
            sum2 += find_symmetry(0, solution);
            y = 0;
            continue;
        }

        N = strlen(line) - 1;

        for (x = 0; x < N; ++x) {
            pattern[y * N + x] = line[x] == '#' ? 1 : 0;
        }

        y++;
    }
    // Remember last figure
    int solution = find_symmetry(0, 0);
    sum1 += solution;
    sum2 += find_symmetry(0, solution);

    fclose(file);

    printf("%d\n", sum1);
    printf("%d\n", sum2);

    return 0;
}

int columns_match(int a, int b)
{
    for (int y = 0; y < M; ++y) {
        if (pattern[y * N + a] != pattern[y * N + b])
            return 0;
    }

    return 1;
}

int rows_match(int a, int b)
{
    for (int x = 0; x < N; ++x) {
        if (pattern[a * N + x] != pattern[b * N + x])
            return 0;
    }

    return 1;
}

int find_symmetry(int flip, int prev) {
    int match;

    if (flip > M * N) {
        fprintf(stderr, "ERROR: No symmetry found\n.");
        exit(1);
    }

    // Check if we have some previous solution defined (part 2)
    if (prev) {
        pattern[flip] = !pattern[flip];
    }

    for (int x = 0; x < N-1; ++x) {
        match = 1; // Assume match
        int i = x;
        int j = x+1;
        while (i >= 0 && j < N) {
            if (!columns_match(i, j)) {
                match = 0;
                break;
            }
            i--;
            j++;
        }

        if (match) {
            int sym_line = x + 1;
            if (sym_line != prev) {
                return sym_line;
            }
        }
    }

    for (int y = 0; y < M-1; ++y) {
        match = 1; // Assume match
        int i = y;
        int j = y+1;
        while (i >= 0 && j < M) {
            if (!rows_match(i, j)) {
                match = 0;
                break;
            }
            i--;
            j++;
        }

        if (match) {
            int sym_line = 100 * (y + 1);
            if (sym_line != prev) {
                return sym_line;
            }
        }
    }

    // Flip back
    pattern[flip] = !pattern[flip];

    return find_symmetry(flip + 1, prev);
}
