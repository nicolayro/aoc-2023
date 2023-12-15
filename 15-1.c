#include <stdio.h>

int main()
{
    char c;
    int tmp = 0, sum = 0;

    FILE *f = fopen("input.txt", "r");
    while ((c = fgetc(f)) != EOF) {
        if (c == ',' || c == '\n') {
            sum += tmp;
            tmp = 0;
        } else
            tmp = (tmp + c) * 17 % 256;
    }
    fclose(f);

    printf("%d\n", sum);
    return 0;
}
