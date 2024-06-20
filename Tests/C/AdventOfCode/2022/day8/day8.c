#include <stdio.h>
#include <string.h>

int line_new[110][110];

int visible_from_left(char line[110], int size, int k, int trans)
{
    int flag = 0;
    int pos;
    int number = 0;
    int tallest = -1;
    for (int i = 0; i < size; i++)
    {
        if (line[i] > tallest)
        {
            tallest = line[i];
            pos = i;
            flag = 1;
        }
    }
    
    if (flag)
    {
        if (trans)
            line_new[pos][k] -= 1;
        else
            line_new[k][pos] -= 1;
        flag = 0;
    }
    
    number++;
    if (pos > 0)
        number += visible_from_left(line, pos, k, trans);
    return number;
}

int visible_from_right(char line[110], int size, int k, int trans)
{
    int flag = 0;
    int pos = size;
    int number = 0;
    int tallest = -1;
    for (int i = strlen(line) - 1; i > size; i--)
    {
        if (line[i] > tallest)
        {
            tallest = line[i];
            pos = i;
            flag = 1;
        }
    }

    if (flag)
    {
        if (trans)
            line_new[pos][k] -= 1;
        else
            line_new[k][pos] -= 1;
        flag = 0;
    }
    
    number++;
    if (pos < strlen(line) - 1)
        number += visible_from_right(line, pos, k, trans);
    return number;
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

    int numberL = 0;
    int numberR = 0;
    int numberU = 0;
    int numberD = 0;

    int trans = 0;
    for (int k = 0; k < line_cnt; k++)
    {
        //LEFT
        numberL += visible_from_left(line[k], strlen(line[k]), k, trans);

        //RIGHT
        int flag = 0;
        int tallest = -1;
        int pos = strlen(line[k]) - 1;
        for (int i = 0; i < strlen(line[k]); i++)
        {
            if (line[k][i] >= tallest)
            {
                tallest = line[k][i];
                pos = i;
                flag = 1;
            }
        }

        if (flag)
        {
            if (trans)
                line_new[pos][k] -= 1;
            else
                line_new[k][pos] -= 1;
            flag = 0;
        }

        numberR++;
        numberR += visible_from_right(line[k], pos, k, trans);
    }

    //transponovana matice
    for (int i = 0; i < line_cnt; i++)
    {
        for (int j = 0; j < strlen(line[i]); j++)
        {
            lineT[j][i] = line[i][j];
        }
    }

    trans = 1;
    for (int k = 0; k < strlen(line[0]); k++)
    {
        //UP
        numberU += visible_from_left(lineT[k], strlen(lineT[k]), k, trans);

        //DOWN
        int flag = 0;
        int tallest = -1;
        int pos = strlen(lineT[k]) - 1;
        for (int i = 0; i < strlen(lineT[k]); i++)
        {
            if (lineT[k][i] >= tallest)
            {
                tallest = lineT[k][i];
                pos = i;
                flag = 1;
            }
        }

        if (flag)
        {
            if (trans)
                line_new[pos][k] -= 1;
            else
                line_new[k][pos] -= 1;
            flag = 0;
        }
        numberD++;
        numberD += visible_from_right(lineT[k], pos, k, trans);
    }

    int sum = 0;
    for (int i = 0; i < line_cnt; i++)
    {
        for (int j = 0; j < strlen(line[0]); j++)
        {
            if (line_new[i][j] < 0)
                line_new[i][j] += 1;
            sum += line_new[i][j];
        }
    }
    
    fprintf(stderr, "LEFT: %d\nRIGHT: %d\nUP: %d\nDOWN: %d\n", numberL, numberR, numberU, numberD);
    fprintf(stderr, "SUM: %d\n", sum);

    printf("Vysledek: %d\n", numberL + numberR + numberU + numberD + sum);

    fclose(input);
    return 0;
}