#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char line[15];


int main()
{
    FILE *input;
    int x_pos = 1;
    int cycle = 1;
    int sum = 0;
    input = fopen("day10.txt", "r");
    while (fgets(line, 15, input) != NULL)
    {
        if (cycle % 1000 == 1)
            printf("#");
        if (line[0] == 'n')
        {
            cycle++;
            if (cycle % 40 == 20)
            {
                sum += cycle * x_pos;
            }

            if (cycle % 40 >= x_pos && cycle % 40 <= x_pos + 2)
                printf("#");
            else
                printf(".");
            if (cycle % 40 == 0)
                printf("\n");

        }
        
        if (line[0] == 'a')
        {
            char tmp_line[10];
            cycle++;

            if (cycle % 40 >= x_pos && cycle % 40 <= x_pos + 2)
                printf("#");
            else
                printf(".");
            if (cycle % 40 == 0)
                printf("\n");

            if (cycle % 40 == 20)
            {
                sum += cycle * x_pos;
            }
            for (int i = 5; i < strlen(line); i++)
            {
                tmp_line[i - 5] = line[i];
                tmp_line[i - 5 + 1] = '\0';
            }
            x_pos += atoi(tmp_line);
            cycle++;

            if (cycle % 40 >= x_pos && cycle % 40 <= x_pos + 2)
                printf("#");
            else
                printf(".");
            if (cycle % 40 == 0)
                printf("\n");

            if (cycle % 40 == 20)
            {
                sum += cycle * x_pos;
            }
        }
    }
    printf("\nSum of strengest signals is: %d\n", sum);

    fclose(input);
    return 0;
}