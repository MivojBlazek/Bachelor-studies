#include <stdio.h>
#include <string.h>

char line[60];
char prev_prev_line[60];
char prev_line[60];
char first_compartment[30];
char second_compartment[30];

char find_union_letter(char line1[], char line2[], char line3[])
{
    for (int i = 0; i < strlen(line1); i++)
    {
        for (int j = 0; j < strlen(line2); j++)
        {
            for (int k = 0; k < strlen(line3); k++)
            {
                if (line1[i] == line2[j] && line2[j] == line3[k] && ((line1[i] >= 'a' && line1[i] <= 'z') || (line1[i] >= 'A' && line1[i] <= 'Z')))
                    return line1[i];
            }
        }
    }
    return 0;
}


int main()
{
    FILE *input;
    int union_char = 0;
    int value_of_union_chars = 0;
    int value_of_badges = 0;


    input = fopen("day3.txt", "r");
    for (int x = 0; fgets(line, 60, input) != NULL; x++)
    {
        if (x % 3 == 2)
        {
            char badge = find_union_letter(line, prev_line, prev_prev_line);
            if (badge >= 'a' && badge <= 'z')
                value_of_badges += badge - 'a' + 1;
            else
                value_of_badges += badge - 'A' + 27;
        }

        for (int i = 0; i < strlen(line) / 2; i++)
        {
            first_compartment[i] = line[i];
            second_compartment[i] = line[(strlen(line) / 2) + i];

            first_compartment[i + 1] = '\0';
            second_compartment[i + 1] = '\0';
        }

        for (int i = 0; i < strlen(first_compartment); i++)
        {
            for (int j = 0; j < strlen(second_compartment); j++)
            {
                if (first_compartment[i] == second_compartment[j])
                {
                    if (first_compartment[i] >= 'a' && first_compartment[i] <= 'z')
                        union_char = first_compartment[i] - 'a' + 1;
                    else
                        union_char = first_compartment[i] - 'A' + 27;
                }
            }
        }
        value_of_union_chars += union_char;
        
        strcpy(prev_prev_line, prev_line);
        strcpy(prev_line, line);
    }
    printf("Sum of things in both compartments of their rucksacks is: %d\n", value_of_union_chars);
    printf("Sum of trio badges is: %d\n", value_of_badges);

    fclose(input);
    return 0;
}