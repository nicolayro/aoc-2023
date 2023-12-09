#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LABEL_SIZE 3
#define MAX_NODES 750

typedef struct Node {
    char label[LABEL_SIZE];
    char left[LABEL_SIZE];
    char right[LABEL_SIZE];
} Node;

char *instructions;

int nodes_length;
Node *nodes;

void parse_input(char *filename);
int traverse(char *source, char *dest);
long long mul_traverse();

int main(int argc, char **argv)
{
    char *filename = argc > 1 ? argv[1] : "input.txt";

    instructions = malloc(271 * sizeof(char));
    nodes = malloc(MAX_NODES * sizeof(Node));

    parse_input(filename);

    int steps1 = traverse("AAA", "ZZZ");
    printf("%d\n", steps1);

    long long steps2 = mul_traverse();
    printf("%lld\n", steps2);

    free(nodes);

    return 0;
}

void read_label(char *string, char *out)
{
    for (int i = 0; i < LABEL_SIZE; ++i) {
        out[i] = string[i];
    }
}

void parse_input(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Unable to open file '%s'.\n", filename);
        exit(1);
    }

    fgets(instructions, 512, file); // Read instructions

    char line[32];
    fgets(line, sizeof(line), file); // Dump emty line

    Node n = {};
    int offset;
    while (fgets(line, sizeof(line), file)) {
        offset = 0;

        read_label(line, n.label);

        while (line[offset++] != '(');
        read_label(line + offset, n.left);

        while (line[offset++] != ',');
        read_label(line + offset + 1, n.right);

        nodes[nodes_length++] = n;
    }

    fclose(file);
}

int is_equal(char *label_a, char *label_b)
{
    for (int i = 0; i < LABEL_SIZE; ++i) {
        if (label_b[i] != '?' && label_a[i] != label_b[i])
            return 0;
    }
    return 1;
}

Node *find_node(char *label)
{
    for (int i = 0; i < nodes_length; ++i) {
        if (is_equal(label, nodes[i].label))
            return &nodes[i];
    }
    return NULL;
}

int traverse(char *source, char *dest)
{
    int steps = 0;
    Node *n = find_node(source);

    for(;;) {
        for (int i = 0; i < strlen(instructions) - 1; ++i) {
            // printf("%4d: %.3s | %.3s %.3s | %c\n", steps, n->label, n->left, n->right, instructions[i]);
            steps++;

            n = instructions[i] == 'L'
                ? find_node(n->left)
                : find_node(n->right);

            if (is_equal(n->label, dest))
                return steps;
        }
    }
}

long long gcd(long long a, long long b)
{
    if (b == 0)
        return a;

    return gcd(b, a % b);
}

long long lcm(long long a, long long b)
{
    return (a / gcd(a, b)) * b;
}

long long mul_traverse()
{
    int num_paths = 0;
    Node **paths;

    paths = malloc(100 * sizeof(Node*));
    for (int i = 0; i < nodes_length; ++i) {
        if (nodes[i].label[2] == 'A') {
            paths[num_paths++] = &nodes[i];
        }
    }

    long long steps = 1;
    for (int i = 0; i < num_paths; ++i) {
        int n = traverse(paths[i]->label, "??Z");
        steps = lcm(steps, n);
        printf("%lld\n", steps);
    }

    return steps;
}
