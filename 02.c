#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_REDS    12
#define NUM_GREENS  13
#define NUM_BLUES   14

#define MAX(a, b)   ((a) > (b)) ? (a) : (b)

int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

int main()
{
    int gameId = 0;
    int sum1 = 0;
    int sum2 = 0;
    int i, possible, red, green, blue;
    long balls;

    char line[256];
    char *filename = "input.txt";
    FILE *file;

    file = fopen(filename, "r");
    while (fgets(line, sizeof(line), file)) {
        gameId++;

        i = 0;
        while (line[i++] != ':'); // Skip first part

        red = 0;
        blue = 0;
        green = 0;
        balls = 0;
        possible = 1;

        for (;i < strlen(line); ++i) {
            if (is_digit(line[i]) && !balls) {
                balls = strtol(line + i, NULL, 10);
            }

            switch (line[i]) {
                case 'r': {
                    if (balls > NUM_REDS)
                        possible = 0;
                    red = MAX(red, balls);
                    balls = 0;
                    break;
                }
                case 'g': {
                    if (balls > NUM_GREENS)
                        possible = 0;
                    green = MAX(green, balls);
                    balls = 0;
                    break;
                }
                case 'b': {
                    if (balls > NUM_BLUES)
                        possible = 0;
                    blue = MAX(blue, balls);
                    balls = 0;
                    break;
                }
            }
        }
        sum1 += possible * gameId;
        sum2 += red * green * blue;
    }

    fclose(file);

    printf("%d\n", sum1);
    printf("%d\n", sum2);

    return 0;

}
