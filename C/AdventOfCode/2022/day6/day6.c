#include <stdio.h>

int main()
{
    char c[14];
    int count;
    FILE *input;
    input = fopen("day6.txt", "r");
    int flag = 0;    

    for (int i = 0; i < 13; i++)
    {
        c[i] - fgetc(input);
    }
    
    for (count = 13; (c[13] = fgetc(input)) != EOF; count++)
    {
        for (int i = 0; i < 13; i++)
        {
            for (int j = i + 1; j < 14; j++)
            {
                if (c[i] == c[j])
                    flag++;
            }
        }
        if (!flag)
            break;
        else
	    flag = 0;

        for (int i = 0; i < 13; i++)
        {
            c[i] = c[i + 1];
        }
    }
    count++;

    printf("Sequence ends on %d index\n", count);
    fclose(input);
    return 0;
}
