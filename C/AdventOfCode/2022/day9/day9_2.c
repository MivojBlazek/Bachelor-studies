#include <stdio.h>
#define SIZE 500

int abs(int x)
{
    if (x < 0)
        return -x;
    return x;
}

char array[300][SIZE];

void find_shortest_way(int *x1, int *y1, int *x2, int *y2, int print, int dir)
{
    while (abs(*x1 - *x2) > 1 || abs(*y1 - *y2) > 1)
    {
        if (dir == 1)//horizontal
        {
            if ((*x1 - *x2) > 1)
            {
                *x2 += 1;
                if (abs(*y1 - *y2) == 1)
                {
                    *y2 = *y1;
                    continue;
                }
            }
            else if ((*x1 - *x2) < -1)
            {
                *x2 -= 1;
                if (abs(*y1 - *y2) == 1)
                {
                    *y2 = *y1;
                    continue;
                }
            }

            if ((*y1 - *y2) > 1)
            {
                *y2 += 1;
            }
            else if ((*y1 - *y2) < -1)
            {
                *y2 -= 1;
            }

            if (print)
                array[*x2][*y2] = '#';
        }
        else if (dir == 2)//vertical
        {
            if ((*y1 - *y2) > 1)
            {
                *y2 += 1;
                if (abs(*x1 - *x2) == 1)
                {
                    *x2 = *x1;
                    continue;
                }
            }
            else if ((*y1 - *y2) < -1)
            {
                *y2 -= 1;
                if (abs(*x1 - *x2) == 1)
                {
                    *x2 = *x1;
                    continue;
                }
            }
            
            if ((*x1 - *x2) > 1)
            {
                *x2 += 1;
            }
            else if ((*x1 - *x2) < -1)
            {
                *x2 -= 1;
            }

            if (print)
                array[*x2][*y2] = '#';
        }
    }
}


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

    int x3 = 50;
    int y3 = (SIZE / 2) + 100;
    int x4 = 50;
    int y4 = (SIZE / 2) + 100;
    int x5 = 50;
    int y5 = (SIZE / 2) + 100;
    int x6 = 50;
    int y6 = (SIZE / 2) + 100;
    int x7 = 50;
    int y7 = (SIZE / 2) + 100;
    int x8 = 50;
    int y8 = (SIZE / 2) + 100;
    int x9 = 50;
    int y9 = (SIZE / 2) + 100;
    int x10 = 50;
    int y10 = (SIZE / 2) + 100;
    array[x10][y10] = '#';

    int print = 0;
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

                    int dir = 1;
                    find_shortest_way(&x2, &y2, &x3, &y3, print, dir);
                    find_shortest_way(&x3, &y3, &x4, &y4, print, dir);
                    find_shortest_way(&x4, &y4, &x5, &y5, print, dir);
                    find_shortest_way(&x5, &y5, &x6, &y6, print, dir);
                    find_shortest_way(&x6, &y6, &x7, &y7, print, dir);
                    find_shortest_way(&x7, &y7, &x8, &y8, print, dir);
                    find_shortest_way(&x8, &y8, &x9, &y9, print, dir);
                    print = 1;
                    find_shortest_way(&x9, &y9, &x10, &y10, print, dir);
                    print = 0;
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

                    int dir = 1;
                    find_shortest_way(&x2, &y2, &x3, &y3, print, dir);
                    find_shortest_way(&x3, &y3, &x4, &y4, print, dir);
                    find_shortest_way(&x4, &y4, &x5, &y5, print, dir);
                    find_shortest_way(&x5, &y5, &x6, &y6, print, dir);
                    find_shortest_way(&x6, &y6, &x7, &y7, print, dir);
                    find_shortest_way(&x7, &y7, &x8, &y8, print, dir);
                    find_shortest_way(&x8, &y8, &x9, &y9, print, dir);
                    print = 1;
                    find_shortest_way(&x9, &y9, &x10, &y10, print, dir);
                    print = 0;
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

                    int dir = 2;
                    find_shortest_way(&x2, &y2, &x3, &y3, print, dir);
                    find_shortest_way(&x3, &y3, &x4, &y4, print, dir);
                    find_shortest_way(&x4, &y4, &x5, &y5, print, dir);
                    find_shortest_way(&x5, &y5, &x6, &y6, print, dir);
                    find_shortest_way(&x6, &y6, &x7, &y7, print, dir);
                    find_shortest_way(&x7, &y7, &x8, &y8, print, dir);
                    find_shortest_way(&x8, &y8, &x9, &y9, print, dir);
                    print = 1;
                    find_shortest_way(&x9, &y9, &x10, &y10, print, dir);
                    print = 0;
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

                    int dir = 2;
                    find_shortest_way(&x2, &y2, &x3, &y3, print, dir);
                    find_shortest_way(&x3, &y3, &x4, &y4, print, dir);
                    find_shortest_way(&x4, &y4, &x5, &y5, print, dir);
                    find_shortest_way(&x5, &y5, &x6, &y6, print, dir);
                    find_shortest_way(&x6, &y6, &x7, &y7, print, dir);
                    find_shortest_way(&x7, &y7, &x8, &y8, print, dir);
                    find_shortest_way(&x8, &y8, &x9, &y9, print, dir);
                    print = 1;
                    find_shortest_way(&x9, &y9, &x10, &y10, print, dir);
                    print = 0;
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
    printf("Number of places visited by tail of rope: %d\n", count); //between 2085 and 2596
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