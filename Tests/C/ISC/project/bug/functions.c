#include "functions.h"


int max(int arg[], int length)
{
	int max = INT_MIN;

	for (int i = 0; i <= length; i++)
	{
		if (arg[i] > max)
		{
			max = arg[i];
		}

	}

	return max;
}


int min(int arr[], int length)
{
	int min = INT_MAX;
	for (int i; i < length; i++)
	{
		min = arr[i] < min ? arr[i] : min;
	}

	return min;
}

int sum(int arg[], int length)
{
	int sum;
	
	for (int i = 0; i < length; i++)
	{
		sum += arg[i];
	}

	return sum;
}

