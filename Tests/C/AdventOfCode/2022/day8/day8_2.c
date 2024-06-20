#include <stdio.h>
#include <string.h>

int visible_from_left(char line[110], int size, int tallest, int k)
{
    int i;
    for (i = size + 1; i < strlen(line); i++)
    {
        if (line[i] >= tallest)
        {
            return i - size;
        }
    }
    return i - size - 1;
}

int visible_from_right(char line[110], int size, int tallest, int k)
{
    int i;
    for (i = size - 1; i > -1; i--)
    {
        if (line[i] >= tallest)
        {
            return size - i;
        }
    }
    return size - i - 1;
}


char line[110][110];
char lineT[110][110];

int main()
{
    FILE *input;
    input = fopen("day8.txt", "r");

    //ulozeni do 2D pole
    int line_cnt;
    for (line_cnt = 0; fgets(line[line_cnt], 110, input) != NULL; line_cnt++)
    {
        if (line[line_cnt][strlen(line[line_cnt]) - 1] == '\n')
        {
            line[line_cnt][strlen(line[line_cnt]) - 1] = '\0';
        }
        
    }

    int number_of_visible = 1;
    int max = 0;

    //transponovana matice
    for (int i = 0; i < line_cnt; i++)
    {
        for (int j = 0; j < strlen(line[i]); j++)
        {
            lineT[j][i] = line[i][j];
        }
    }

    for (int i = 0; i < line_cnt; i++)
    {
        for (int j = 0; j < strlen(line[i]); j++)
        {
            int tallest = line[i][j];
            number_of_visible = 1;

            number_of_visible *= visible_from_left(line[i], j, tallest, i);
            number_of_visible *= visible_from_right(line[i], j, tallest, i);

            number_of_visible *= visible_from_left(lineT[j], i, tallest, j);
            number_of_visible *= visible_from_right(lineT[j], i, tallest, j);

            if (number_of_visible > max)
            {
                max = number_of_visible;
            }
        }
    }
    printf("Maximum visible distance is: %d\n", max);

    fclose(input);
    return 0;
}