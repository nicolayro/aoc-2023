#include <stdio.h>
#include <stdlib.h>

int seq_len;
int sequence[21];

void parse_sequence(char *l)
{
    int n;
    char *s, *e;

    seq_len = 0;
    for (s = l; *s != '\n'; s = e) {
        n = strtol(s, &e, 10);
        sequence[seq_len++] = n;

        if (s == e)
            return;
    }
}

int next_value(int *seq, int len)
{
    int all_zeroes = 1;
    for (int i = 0; i < len; ++i) {
        if (seq[i] != 0) {
            all_zeroes = 0;
            break;
        }
    }
    if (all_zeroes)
        return 0;

    int next_seq[len - 1];
    for (int i = 0; i < len - 1; ++i) {
        next_seq[i] = seq[i+1] - seq[i];
    }

    return seq[len-1] + next_value(next_seq, len - 1);
}

void reverse(int *arr, int len)
{
    int i = 0, j = len - 1;
    while (i < j) {
        int tmp = arr[i];
        arr[i++] = arr[j];
        arr[j--] = tmp;
    }
}

int main()
{
    int sum1 = 0;
    int sum2 = 0;

    char line[256];
    FILE *f = fopen("input.txt", "r");

    while (fgets(line, sizeof(line), f)) {
        parse_sequence(line);
        sum1 += next_value(sequence, seq_len);

        reverse(sequence, seq_len);
        sum2 += next_value(sequence, seq_len);
    }

    fclose(f);

    printf("%d\n", sum1);
    printf("%d\n", sum2);

    return 0;
}
