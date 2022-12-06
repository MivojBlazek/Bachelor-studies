#include <stdio.h>

int main()
{
    char c[4];
    int count;
    FILE *input;
    input = fopen("day6.txt", "r");
    
    c[0] = fgetc(input);
    c[1] = fgetc(input);
    c[2] = fgetc(input);
    for (count = 3; (c[3] = fgetc(input)) != EOF; count++)
    {
        if (c[0] != c[1] && c[0] != c[2] && c[0] != c[3] && c[1] != c[2] && c[1] != c[3] && c[2] != c[3])
            break;
        c[0] = c[1];
        c[1] = c[2];
        c[2] = c[3];
    }
    count++;
    printf("Sequence ends on %d index\n", count);
    fclose(input);
    return 0;
}