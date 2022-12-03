#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "ERROR\n");
		return 1;
	}
	
	int size_of_str = 0;
///	int read_char;
//	int size_of_str = sizeof(argv[1])/sizeof(int);
//	printf("%d", size_of_str);
	char read_str[100];
	for (int t = 0; t < 100; t++)
	{
		read_str[t] = '\0';
	}
	for (int i = 0; (/*read_char = */argv[1][i]) != '\0'; i++)
	{
		read_str[i] = /*read_char*/argv[1][i];
		size_of_str = i + 1;
	}
	printf("%s\n", read_str);
/*	char read_str[size_of_str];
	for (int a = 0; a < size_of_str; a++)
	{
		read_str[a] = pizza[a];
	}
*/	printf("%d\n", size_of_str);
////	printf("%s\n", read_str);
//	char read_str[] = *argv[1]
//	printf("%s", read_str);

	int count = 0;
	int list_char;
	for (int i = 0; (list_char = getchar()) != EOF; i++)
	{
		if (read_str[count] == list_char)
		{
			count++;
			if (count == size_of_str)
			{
				break;
			}
		}
		else
		{
			count = 0;
		}
		/*for (int j = 0; j < size_of_str; j++)
		{
			if (read_str[j] == list_char)
			{
				count++;
			}
		}*/
	}
	printf("%s\n", read_str);
	if (count == size_of_str)
	{
		printf("Je tam %d\n", count);
	} 
	return 0;
}
