#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long x, y;
} Position;

long N, M;
int galaxies_len;
Position galaxies[512];

void cosmic_expansion(int expansion)
{
    for (long x = 0; x < N * expansion * 2; ++x) {
        bool empty = true;
        for (int i = 0; i < galaxies_len; ++i) {
            if (galaxies[i].x == x) {
                empty = false;
            }
        }
        if (!empty)
            continue;

        for (int i = 0; i < galaxies_len; ++i) {
            if (galaxies[i].x > x)
                galaxies[i].x += expansion;
        }
        x += expansion; // Jump the new 'column'
    }

    for (long y = 0; y < M * expansion * 2; ++y) {
        bool empty = true;
        for (int i = 0; i < galaxies_len; ++i) {
            if (galaxies[i].y == y) {
                empty = false;
            }
        }
        if (!empty) continue;

        for (int i = 0; i < galaxies_len; ++i) {
            if (galaxies[i].y > y)
                galaxies[i].y += expansion;
        }
        y += expansion; // Jump the new 'column'
    }
}

int main(int argc, char **argv)
{
    char *filename = argc > 1 ? argv[1] : "input.txt";

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "ERROR: Unable to open file '%s'.\n", filename);
        exit(1);
    }

    char c;
    long x, y = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        for (x = 0; x < strlen(line); ++x) {
            if (line[x] == '#')
                galaxies[galaxies_len++] = (Position) {x, y};
        }
        y++;
    }

    N = strlen(line);
    M = y;

    cosmic_expansion(1000000-1); // '1' for part 1 solution
    long long sum = 0;
    for (int i = 0; i < galaxies_len - 1; ++i) {
        for (int j = i; j < galaxies_len; ++j) {
            Position a = galaxies[i];
            Position b = galaxies[j];
            long distance = labs((b.x - a.x)) + labs((b.y - a.y));
            sum += distance;
        }
    }

    printf("%lld\n", sum);

    return 0;
}
