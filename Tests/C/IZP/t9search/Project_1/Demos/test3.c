#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		fprintf(stderr, "ERROR\n");
		return 1;
	}
	int read_char;
	int size_of_str = sizeof(argv[1])/sizeof(int);
	char read_str[size_of_str];
	for (int  i = 0; (read_char = argv[1][i]) != '\0'; i++)
	{
		read_str[i] = read_char;
	}
	printf("%s\n", read_str);

	int list_char;
	for (int i = 0; (list_char = getchar()) != EOF; i++)
	{
//		if (find == list_char)
//		{
			putchar(list_char);
//			printf("\nANO\n");
//		}
	} 
	return 0;
}
