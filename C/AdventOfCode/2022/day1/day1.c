#include <stdio.h>
#include <stdlib.h>

char line[20];
int arr_of_kcal_sum[3000];

int main()
{

    FILE *input;
    int kcal_sum = 0;
    int arr_index = 0;
    int max_of_elves = 0;

    input = fopen("day1.txt", "r");
    for (int i = 0; i < 3000; i++)
    {
        if(fgets(line, 20, input) == EOF)
            break;
        if (line[0] == '\n')
        {
            arr_of_kcal_sum[arr_index++] = kcal_sum;
            kcal_sum = 0;
            //arr_index++;
        }
        kcal_sum += atol(line);
    }
    arr_of_kcal_sum[arr_index++] = kcal_sum;


    int kcal_of_three_elves = 0;

    for (int i = 0; i < 3; i++)
    {
        int idx_of_max = 0;
        for (int j = 0; j < 3000; j++)
        {
            if (arr_of_kcal_sum[j] > max_of_elves)
            {
                max_of_elves = arr_of_kcal_sum[j];
                idx_of_max = j;
            }
        }
        kcal_of_three_elves += max_of_elves;
        max_of_elves = 0;
        arr_of_kcal_sum[idx_of_max] = 0;
    }
    printf("Maximum kcal of three elves: %d\n", kcal_of_three_elves);

    fclose(input);

    return 0;
}