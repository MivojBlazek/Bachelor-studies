#include <stdio.h>
#include <stdlib.h>

char line_char[2500][3];

int main()
{
    int points = 0;
    FILE *input;
    
    input = fopen("day2.txt", "r");
    for (int j = 0; j < 2500; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            line_char[j][i] = fgetc(input);
        }
        if (fgetc(input) == '\n')
            continue;
    }
    for (int i = 0; i < 2500; i++)
    {
        if (line_char[i][0] == 'A')
        {
            if (line_char[i][2] == 'X')
            {
                points += 3;
            }
            if (line_char[i][2] == 'Y')
            {
                points += 4;
            }
            if (line_char[i][2] == 'Z')
            {
                points += 8;
            }
        }
        if (line_char[i][0] == 'B')
        {
            if (line_char[i][2] == 'X')
            {
                points += 1;
            }
            if (line_char[i][2] == 'Y')
            {
                points += 5;
            }
            if (line_char[i][2] == 'Z')
            {
                points += 9;
            }
        }
        if (line_char[i][0] == 'C')
        {
            if (line_char[i][2] == 'X')
            {
                points += 2;
            }
            if (line_char[i][2] == 'Y')
            {
                points += 6;
            }
            if (line_char[i][2] == 'Z')
            {
                points += 7;
            }
        }
    }
    printf("Points with algorithm to choose W/D/L: %d\n", points);
    
    fclose(input);

    return 0;
}