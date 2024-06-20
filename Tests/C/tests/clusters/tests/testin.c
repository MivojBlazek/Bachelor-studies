#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct obj_t
{
	int id;
	float x;
	float y;
};
struct cluster_t
{
	int size;
	int capacity;
	struct obj_t *obj;
};

int load_clusters(char *filename, int **arr)
{
	FILE *input;
	char first_line[] = "count=";
	char line[100];
	input = fopen(filename, "r");
	if (input == NULL)
	{
		return 0;
	}
	int sizeF = sizeof(first_line) / sizeof(char);
	int size = sizeof(line) / sizeof(char);
	fgets(line, sizeF, input);
	for (int i = 0; i < sizeF; i++)
	{
		if (first_line[i] != line[i])
	        {
			return 0;
		}
	}
	fgets(line, size, input);
	int line_cnt = atoi(line);
	int array[line_cnt][3];
	for (int i = 0; fgets(line, size, input) != NULL; i++)
	{
		char *token = strtok(line, " ");
		for (int j = 0; token; j++)
		{
			array[i][j] = atoi(token);
			token = strtok(NULL, " ");
		}
	}
	/*for (int i = 0; i < line_cnt; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%d ", array[i][j]);
		}
		printf("\n");
	}*/
	fclose(input);
	return line_cnt;
}

int main()
{
	int *arr[100];
	char file[] = "objekty.txt";
	int x = load_clusters(file, (arr));
	printf("%d\n", x);
	/*for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}*/
	return 0;
}
