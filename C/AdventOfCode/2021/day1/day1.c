#include <stdio.h>
#include <stdlib.h>

char line[10];
int arr_of_depths[2010];

int find_larger(int first, int second, int increase)
{
    if (first >= second)
        return increase;
    return ++increase;
}

int main()
{
    int num_of_increases = 0;
    int j = 0;

    FILE *input;
    input = fopen("day1.txt", "r");

    int depths = 0;
    int depths2 = 0;
    int depths3 = 0;
    int depths4 = 0;
    int i;
    for (i = 0; fgets(line, 10, input) != NULL; i++)
    {
        if (i % 4 != 3)
            depths4 += atol(line);
        else
        {
            arr_of_depths[j++] = depths4;
            depths4 = 0;
        }

        if (i % 4 != 2)
            depths3 += atol(line);
        else
        {
            arr_of_depths[j++] = depths3;
            depths3 = 0;
        }
        
        if (i % 4 != 1)
            depths2 += atol(line);
        else
        {
            arr_of_depths[j++] = depths2;
            depths2 = 0;
        }

        if (i % 4 != 0)
            depths += atol(line);
        else
        {
            arr_of_depths[j++] = depths;
            depths = 0;
        }
    }
    switch (i % 4)
    {
        case 0:
            arr_of_depths[j] = depths;
            break;
        case 1:
            arr_of_depths[j] = depths2;
            break;
        case 2:
            arr_of_depths[j] = depths3;
            break;
        case 3:
            arr_of_depths[j] = depths4;
            break;
    }

    for (int i = 3; i < 2010 - 1; i++)
    {
        num_of_increases = find_larger(arr_of_depths[i], arr_of_depths[i + 1], num_of_increases);
    }
    printf("Depth is increasing %d times\n", num_of_increases);

    fclose(input);
    return 0;
}