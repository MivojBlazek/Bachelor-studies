#include <stdio.h>
#include <string.h>

//char line[50];
char tower[9][60];
char final_str[9];

int main()
{
	int input_move, input_from, input_to;
	char c1, c2, c3, c4, c5, c6, c7, c8, c9;
	FILE *input;
	input = fopen("day5.txt", "r");
	for (int i = 0; fscanf(input, "[%c] [%c] [%c] [%c] [%c] [%c] [%c] [%c] [%c]", c1, c2, c3, c4, c5, c6, c7, c8, c9) != 0; i++)
	{
		tower[0][i] = c1;
		tower[1][i] = c2;
		tower[2][i] = c3;
		tower[3][i] = c4;
		tower[4][i] = c5;
		tower[5][i] = c6;
		tower[6][i] = c7;
		tower[7][i] = c8;
		tower[8][i] = c9;
	}
	
	//debug test
	//
	for (int i = 0; i < 9; i++)
	{
		fprintf(stderr, "%d. ", i);
		for (int j = 0; j < 60; j++)
		{
			fprintf(stderr, "%c ", tower[i][j]);
		}
		fprintf(stderr, "\n");
	}
	//
	
	while (fscanf(input, "move %d from %d to %d", input_move, input_from, input_to) != EOF)
	{
		for (int i = 0; i < input_move; i++)
		{
			strncat(tower[input_to - 1], &tower[input_from - 1][0], 1);
		}
	}

	for (int i = 0; i < 9; i++)
	{
		strncat(final_str, &tower[i][strlen(tower[i]) - 1], 1);
	}
	printf("Final string is: %s\n", final_str);
	fclose(input);
	return 0;
}
