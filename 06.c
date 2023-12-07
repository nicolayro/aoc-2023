#include <stdio.h>
#include <stdlib.h>

#define NUM_RACES 4

typedef struct {
    long long time, record;
} Race;

Race races[NUM_RACES]; // List of all races

long part1();
long long part2();

int main()
{
    char *filename = "input.txt";
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file '%s'.", filename);
        return 1;
    }

    char line[64];
    char *start, *end;
    long number;
    int offset, curr_race;

    // Parse times
    offset = 0;
    curr_race = 0;
    fgets(line, sizeof(line), file);
    while (line[offset++] != ':'); // skip first part including ':'
    for (start = line + offset;; start = end) {
        number = strtol(start, &end, 10);
        if (start == end)
            break;

        races[curr_race++].time = number;
    }

    // Parse record distances
    offset = 0;
    curr_race = 0;
    fgets(line, sizeof(line), file);
    while (line[offset++] != ':'); // skip first part including ':'
    for (start = line + offset;; start = end) {
        number = strtol(start, &end, 10);
        if (start == end)
            break;

        races[curr_race++].record = number;
    }

    fclose(file);

    printf("%ld\n", part1());
    printf("%lld\n", part2());

    return 0;
}

long num_wins(Race *race)
{
    long sum = 0;
    for (long long hold = 1; hold < race->time; ++hold) {
        long long distance = (race->time - hold) * hold;
        if (distance > race->record)
            sum++;
    }
    return sum;
}

long part1()
{
    long result = 1;
    for (int i = 0; i < NUM_RACES; ++i) {
        result *= num_wins(&races[i]);
    }
    return result;
}

long long concat(long long a, long long b)
{
    long long pow = 10;
    while (pow < b)
        pow *= 10;
    return a * pow + b;
}

long long part2()
{
    Race correct = {0};
    for (int i = 0; i < NUM_RACES; ++i) {
        Race r = races[i];
        correct.time = concat(correct.time, r.time);
        correct.record = concat(correct.record, r.record);
    }

    return num_wins(&correct);
}
