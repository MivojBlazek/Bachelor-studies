#include <stdio.h>
#include <string.h>

char tower[9][100];
char tmp[100];
char final_str[9];

int main(int argc)
{
    int input_move, input_from, input_to;
    char c;
    FILE *input;
    input = fopen("day5.txt", "r");

    //nacte uvodni pozici pismen
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; (c = fgetc(input)) != '\n'; j++)
        {
            if (c >= 'A' && c <= 'Z')
            {
                strncat(tower[j / 4], &c, 1);
            }
        }
    }

    //otoci radky (reverse)
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < strlen(tower[i]) / 2; j++)
        {
            char tmp_c = tower[i][j];
            tower[i][j] = tower[i][strlen(tower[i]) - 1 - j];
            tower[i][strlen(tower[i]) - 1 - j] = tmp_c;
        }
        tower[i][strlen(tower[i])] = '\0';
    }

    //cte zbytek souboru a provadi pohyby
    while (fscanf(input, "move %d from %d to %d\n", &input_move, &input_from, &input_to) != EOF)
    {
        input_from--;
        input_to--;
        if (argc == 2)
        {
            for (int i = 0; i < input_move; i++)
            {
                strncat(tower[input_to], &tower[input_from][strlen(tower[input_from]) - 1], 1);
                tower[input_from][strlen(tower[input_from]) - 1] = '\0';
                tower[input_to][strlen(tower[input_to])] = '\0';
            }
        }
        else
        {
            for (int i = 0; i < input_move; i++)
            {
                strncat(tmp, &tower[input_from][strlen(tower[input_from]) - 1], 1);
                tower[input_from][strlen(tower[input_from]) - 1] = '\0';
            }

            //otoci radky (reverse)
            for (int j = 0; j < strlen(tmp) / 2; j++)
            {
                char tmp_char = tmp[j];
                tmp[j] = tmp[strlen(tmp) - 1 - j];
                tmp[strlen(tmp) - 1 - j] = tmp_char;
            }

            strcat(tower[input_to], tmp);

            tower[input_to][strlen(tower[input_to])] = '\0';
        }
        for (int i = 0; i < 100; i++)
        {
            tmp[i] = '\0';
        }
    }
    
    //vypise posledni pismena jako string
    for (int i = 0; i < 9; i++)
    {
        strncat(final_str, &tower[i][strlen(tower[i]) - 1], 1);
    }
    printf("Final string is: %s\n", final_str);
    fclose(input);
    return 0;
}