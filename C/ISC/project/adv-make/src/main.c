#include <stdio.h>
#include <stdlib.h>

#include "functions.h"


int main()
{
	int arrayLength = 0;
	scanf("%d", &arrayLength);

	int *array = (int *)malloc(arrayLength * sizeof(int));

	for (int i = 0; i < arrayLength; i++)
	{
		scanf("%d",&array[i]);
	}

	int a = sum(array, arrayLength);
	int b = min(array, arrayLength);
	int c = max(array, arrayLength);

	printf(
		"Sum: %d, Min: %d, Max: %d\n", a, b, c
	);

	arrayLength = 0;
	for (int i = 0; i < arrayLength; i++)
	{
		array[i] = 0;
	}

	free(array);

	return 0;
}
