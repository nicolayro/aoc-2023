#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int focal_len;
    int label_len;
    char label[8];
} Lens;

typedef struct {
    int count;
    Lens *lenses;
} Box;

Box *boxes;

int hash(char *str, int n)
{
    int curr;
    for (int i = 0; i < n; ++i) {
        curr = ((curr + str[i]) * 17) % 256; }
    return curr;
}

void add_lens(char *label, int len, int focal_len)
{
    Box *box = &boxes[hash(label, len)];

    // Look for existing entry
    for (int i = 0; i < box->count; ++i) {
        Lens *lens = &box->lenses[i];
        if (lens->label_len == len && strncmp(lens->label, label, len) == 0) {
            lens->focal_len = focal_len;
            return;
        }
    };

    // No existing entry - create one
    Lens *l = &box->lenses[box->count++];
    strncpy(l->label, label, len);
    l->label_len = len;
    l->focal_len = focal_len;
}

void remove_lens(char *label, int len)
{
    Box *box = &boxes[hash(label, len)];

    for (int i = 0; i < box->count; ++i) {
        Lens *lens = &box->lenses[i];
        if (lens->label_len == len && strncmp(lens->label, label, len) == 0) {
            for (int j = i; j < box->count - 1; ++j) {
                box->lenses[j] = box->lenses[j + 1];
            }
            box->count--;
            return;
        }
    };
}

int main()
{
    boxes = malloc(256 * sizeof(Box));
    for (int i = 0; i < 256; ++i) {
        boxes[i].count = 0;
        boxes[i].lenses = malloc(8 * sizeof(Lens));
    }

    FILE *f = fopen("input.txt", "r");

    char c;
    int len = 0;
    char label[8];
    while ((c = fgetc(f)) != EOF) {
        if (c == ',' || c == '\n') {
            len = 0;
        } else if (c == '=') {
            int focal_len = fgetc(f) - '0';
            add_lens(label, len, focal_len);
        } else if (c == '-') {
            remove_lens(label, len);
        } else {
            label[len++] = c;
        }
    }
    fclose(f);

    int sum = 0;
    for (int i = 0; i < 256; ++i) {
        Box *box = &boxes[i];
        for (int j = 0; j < box->count; ++j) {
            int focus_power = (i+1) * (j+1) * box->lenses[j].focal_len;
            sum += focus_power;
        }
    }

    for (int i = 0; i < 256; ++i) {
        free(boxes[i].lenses);
    }
    free(boxes);

    printf("%d\n", sum);

    return 0;
}
