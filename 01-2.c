#include <stdio.h>
#include <string.h>

char *numbers[9] = {
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
};

int main()
{
    char c, buf[64], first = 0, last = 0, *num;
    int sum = 0;

    while (fgets(buf, sizeof(buf), stdin)) {
        for (int i = 0; i < sizeof(buf); ++i) {
            c = buf[i];
            if (c == '\n') {
                sum += 10 * first + last;
                first = 0;
                last = 0;
                break;
            }
            if (c > '0' && c <= '9') {
                first = !first ? c - '0' : first;
                last = c - '0';
            } else {
                for (int j = 0; j < 9; ++j) {
                    num = numbers[j];
                    if (strncmp(buf + i, num, strlen(num)) == 0) {
                        first = !first ? j + 1 : first;
                        last = j + 1;
                        break;
                    }
                }
            }

        }
    }

    printf("%d\n", sum);

    return 0;
}
