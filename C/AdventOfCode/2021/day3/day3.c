#include <stdio.h>
#include <stdlib.h>

int arr[12][1000];

int main()
{
    for (int i = 1; i <= 12; i++)
    {
        FILE *file = fopen("./day3.txt", "r");

        //repeat for another line
        for (int num_of_line = 0; num_of_line < 1000; num_of_line++)
        {
            //get character
            int character;
            for (int j = 0; j < i; j++)
            {
                character = fgetc(file);
            }
            
            //save character to array
            arr[i - 1][num_of_line] = character - 48;
            
            //throw another characters in line 
            do
            {
                character = fgetc(file);
            }while(character == '0' || character == '1');

            if (character == EOF)
            {
                break;
            }
        }

        fclose(file);
    }
    
    //calculate nums
    for (int i = 0; i < 12; i++)
    {
        int value = 0;
        for (int j = 0; j < 1000; j++)
        {
            value += arr[i][j];
        }
        if (value >= 500)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
    printf("\n");
    return 0;
}