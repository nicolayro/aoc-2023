#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char c, first = 0, last = 0, buf[16];
    int sum;

    while ((c = getchar()) != EOF) {
        if (c > '0' && c <= '9') {
            if (!first)
                first = c - '0';
            last = c - '0';
        } else if (c == '\n') {
            sum += 10 * first + last;
            first = 0;
            last = 0;
        }
    }

    printf("%d\n", sum);
    return 0;
}
