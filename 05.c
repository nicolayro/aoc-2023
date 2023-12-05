#include <stdio.h>
#include <stdlib.h>

#define NUM_MAPS    7
#define NUM_SEEDS   10
#define MAX_RULES   64
#define BUFF_SIZE   256

#define MIN(a, b)   ((a) < (b)) ? (a) : (b)

typedef struct {
    long start;
    long length;
} Seed;

typedef struct {
    long dest;
    long source;
    long range;
} Rule;

typedef struct {
    int length;
    Rule *rules;
} RuleArray;

int is_digit(char c);
void parse_seeds(Seed *seeds, char *line);
void parse_rule(Rule *rule, char *line);
long get_location(RuleArray *maps, long seed);

int main(int argc, char** argv)
{

    char *filename = "input.txt";
    if (argc == 2) {
        filename = argv[1];
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("ERROR: Unable to open file '%s'.\n", filename);
        return 1;
    }

    char line[BUFF_SIZE];
    fgets(line, BUFF_SIZE, file);

    Seed *seeds = malloc(NUM_SEEDS * sizeof(Seed));
    parse_seeds(seeds, line);

    int curr_map = -1; // Makes things easier.
    RuleArray *maps = malloc(NUM_MAPS * sizeof(RuleArray));

    while (fgets(line, BUFF_SIZE, file)) {
        if (line[0] == '\n') {
            curr_map++;
            maps[curr_map].length = 0;
            maps[curr_map].rules = malloc(MAX_RULES * sizeof(Rule));
        } else if (is_digit(line[0])) {
            int idx = maps[curr_map].length;
            Rule *rules = maps[curr_map].rules;
            parse_rule(&rules[idx], line);
            maps[curr_map].length++;
        }
    }

    fclose(file);

    long location;
    long answer = 99999999999l;
    for (int i = 0; i < NUM_SEEDS; ++i) {
        for (long seed = seeds[i].start; seed < seeds[i].start + seeds[i].length; ++seed) {
            location = get_location(maps, seed);
            answer = MIN(location, answer);
        }
        printf("Seed %d complete: %zu\n", i, answer);
    }

    for (int i = 0; i < NUM_MAPS; ++i)
        free(maps[i].rules);
    free(maps);
    free(seeds);

    printf("%zu\n", answer);

    return 0;
}

int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

void parse_seeds(Seed *seeds, char *line)
{
    long start, length;

    int idx = 0;
    while (line[idx++] != ':'); // Skip until and including ':'

    char *s, *e;
    int num_parsed = 0;
    for (s = line + idx; num_parsed < NUM_SEEDS ; s = e) {
        start = strtol(s, &e, 10);
        if (s == e)
            break;

        s = e;
        length = strtol(s, &e, 10);

        seeds[num_parsed].start = start;
        seeds[num_parsed].length = length;
        num_parsed++;
    }
}

void parse_rule(Rule *rule, char *line)
{
    char *a = line, *b = NULL;

    long dest = strtol(a, &b, 10);
    long source = strtol(b, &a, 10);
    long range = strtol(a, &b, 10);

    rule->dest = dest;
    rule->source = source;
    rule->range = range;
}

long get_location(RuleArray *maps, long seed) {
    long temp = seed;
    for (int m = 0; m < NUM_MAPS; ++m) {
        for (int r = 0; r < maps[m].length; ++r) {
            Rule *rule = &maps[m].rules[r];
            if (temp >= rule->source && temp < rule->source + rule->range) {
                temp = rule->dest + (temp - rule->source);
                break;
            }
        }
    }
    return temp;
}

