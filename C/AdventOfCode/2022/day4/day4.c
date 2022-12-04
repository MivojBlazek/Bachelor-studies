#include <stdio.h>

char line[15];

int contains(int first_from, int first_to, int second_from, int second_to, int *overlap)
{
    if (first_from <= second_from && first_to >= second_to)
    {
        *overlap += 1;
        return 1;
    }
    else if (first_from >= second_from && first_to <= second_to)
    {
        *overlap += 1;
        return 1;
    }
    else if (second_from <= first_to && second_from >= first_from)
        *overlap += 1;
    else if (first_from <= second_to && first_from >= second_from )
        *overlap += 1;
    return 0;
}

int main()
{
    FILE *input;
    input = fopen("day4.txt", "r");

    int pairs = 0;
    int pairs_overlap = 0;
    int first_from, first_to, second_from, second_to;
    while (fscanf(input, "%d-%d,%d-%d\n", &first_from, &first_to, &second_from, &second_to) != EOF)
    {
        pairs += contains(first_from, first_to, second_from, second_to, &pairs_overlap);
    }
    printf("%d pairs does one range fully contains the second one\n", pairs);
    printf("%d pairs overlap\n", pairs_overlap);

    fclose(input);
    return 0;
}