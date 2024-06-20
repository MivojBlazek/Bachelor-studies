#include <stdio.h>

int main()
{
	int c;
	for (int i = 0; (c = getchar()) != EOF; i++)
	{
		//c = getchar();
		putchar(c);
	} 
	return 0;
}
