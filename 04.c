#include <stdio.h>
#include <stdlib.h>

int main()
{
    int sum1 = 0;
    int sum2 = 0;

    int winning_count = 0;
    int winning[10];

    int current_copy = 0;
    int copies[216];

    long number;
    int i, hits;
    char *start, *end;

    // Add original scratch cards
    for (int idx = 0; idx < 216; ++idx)
        copies[idx] = 1;

    char line[128];
    FILE *file;
    char *filename = "input.txt";

    file = fopen(filename, "r");
    while (fgets(line, sizeof(line), file)) {
        i = 0;
        winning_count = 0;
        current_copy++;

        while (line[i++] != ':'); // Skip until and including ':'

        // Parse winning numbers
        for (start = line + i; *start != '|'; start = end) {
            number = strtol(start, &end, 10);
            if (start == end)
                break;

            winning[winning_count++] = number;
        }

        while (line[i++] != '|'); // Skip until and including '|'

        // Parse actual numbers and count winners
        hits = 0;
        for (start = line + i; *start != '\n'; start = end) {
            number = strtol(start, &end, 10);
            if (start == end)
                break;

            for (int j = 0; j < winning_count; ++j) {
                if (number == winning[j]) {
                    hits++;
                    break;
                }
            }
        }

        // Add copies based on hits
        for (int j = 0; j < hits; ++j)
            copies[current_copy + j + 1] += copies[current_copy];

        sum1 += 1 << hits >> 1;
        sum2 += copies[current_copy];
    }

    fclose(file);

    printf("%d\n", sum1);
    printf("%d\n", sum2);

    return 0;
}
