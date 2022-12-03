#include <stdio.h>
int pole[5];
int main()
{
	for (int i = 0; i < 5; i++)
		printf("%d ", pole[i]);
	printf("%d", EOF);
	return 0;
}
