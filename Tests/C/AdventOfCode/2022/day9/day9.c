#include <stdio.h>
#define SIZE 500

int abs(int x)
{
    if (x < 0)
        return -x;
    return x;
}

char array[300][SIZE];

int main()
{
    char direction;
    int size;
    FILE *input;
    input = fopen("day9.txt", "r");

    //start position
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            array[i][j] = '.';
        }
    }
    int i = 50;
    int j = (SIZE / 2) + 100;
    int x2 = 50;
    int y2 = (SIZE / 2) + 100;

    array[x2][y2] = '#';

    //stepping away
    while (fscanf(input, "%c %d", &direction, &size) != EOF)
    {
        if (direction == 'L')
        {
            for (int k = 1; k <= size; k++)
            {
                if (abs((i - k) - x2) > 1)
                {
                    y2 = j;
                    x2 = i - k + 1;

                    array[x2][y2] = '#';
                }
            }
            i -= size;
        }
        if (direction == 'R')
        {
            for (int k = 1; k <= size; k++)
            {
                if (abs((i + k) - x2) > 1)
                {
                    y2 = j;
                    x2 = i + k - 1;

                    array[x2][y2] = '#';
                }
            }
            i += size;
        }
        if (direction == 'U')
        {
            for (int k = 1; k <= size; k++)
            {
                if (abs((j - k) - y2) > 1)
                {
                    x2 = i;
                    y2 = j - k + 1;

                    array[x2][y2] = '#';
                }
            }
            j -= size;
        }
        if (direction == 'D')
        {
            for (int k = 1; k <= size; k++)
            {
                if (abs((j + k) - y2) > 1)
                {
                    x2 = i;
                    y2 = j + k - 1;

                    array[x2][y2] = '#';
                }
            }
            j += size;
        }
    }
    int count = 0;
    for (int h = 0; h < 300; h++)
    {
        for (int g = 0; g < SIZE; g++)
        {
            if (array[h][g] == '#')
            {
                count++;
            }
        }
    }
    printf("Number of places visited by tail of rope: %d\n", count);
/*
//!test
fprintf(stdout, "\n");
for (int h = 0; h < 300; h++)
{
    for (int g = 0; g < SIZE; g++)
    {
        fprintf(stdout, "%c", array[h][g]);
    }
    fprintf(stdout, "\n");
}
fprintf(stdout, "\n");
//!test
*/
    fclose(input);
    return 0;
}