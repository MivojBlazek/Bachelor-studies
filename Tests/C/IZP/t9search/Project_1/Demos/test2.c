#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		fprintf(stderr, "ERROR\n");
		return 1;
	}
	char find = argv[1][0];
	int c;
	for (int i = 0; (c = getchar()) != EOF; i++)
	{
		if (find == c)
		{
			putchar(c);
			printf("\nANO\n");
		}
	} 
	return 0;
}
