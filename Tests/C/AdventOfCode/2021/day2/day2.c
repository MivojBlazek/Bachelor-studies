#include <stdio.h>

char line[12];

int main()
{
    char forward[] = "forward ";
    char down[] = "down ";
    char up[] = "up ";

    int units_forward = 0;
    long units_down = 0;

    int forward_check = 0;
    int down_check = 0;
    int up_check = 0;

    long aim = 0;

    FILE *input;
    input = fopen("day2.txt", "r");
    while (fgets(line, 12, input) != NULL)
    {
        for (int i = 0; i < sizeof(forward)/sizeof(char); i++)
        {
            if (forward[i] != line[i])
                break;
            forward_check++;
        }
        for (int i = 0; i < sizeof(down)/sizeof(char); i++)
        {
            if (down[i] != line[i])
                break;
            down_check++;
        }
        for (int i = 0; i < sizeof(up)/sizeof(char); i++)
        {
            if (up[i] != line[i])
                break;
            up_check++;
        }

        if (forward_check)
        {
            units_forward += (int)line[sizeof(forward)/sizeof(char) - 1] - '0';
            units_down += (aim * ((int)line[sizeof(forward)/sizeof(char) - 1] - '0'));
        }
        if (down_check)
            aim += (int)line[sizeof(down)/sizeof(char) - 1] - '0';
        if (up_check)
            aim -= (int)line[sizeof(up)/sizeof(char) - 1] - '0';

        fprintf(stderr, "%ld %d\n", aim, forward_check);
        forward_check = 0;
        down_check = 0;
        up_check = 0;
    }
    printf("Submarine went %ld units down and %d units forward\nMultiply of these numbers is: %ld\n", units_down, units_forward, units_down * units_forward);

    fclose(input);
    return 0;
}