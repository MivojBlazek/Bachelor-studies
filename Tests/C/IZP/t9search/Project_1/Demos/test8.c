#include <stdio.h>

int main(int argc, char *argv[])
{

// FUNKCNI VERZE PRO HLEDANI PRESNEHO SUBSTRINGU V SEZNAMU A VYPISU JMENA S CISLEM


/** KONTROLA JESTLI MAME DOSTATECNY POCET ARGUMENTU **/
/** POKUD JE PROGRAM SPUSTENY BEZ ARGUMENTU, MA SE VYTISKNOUT SEZNAM.TXT CELY **/
	if (argc < 2)
	{
		fprintf(stderr, "ERROR\n");
		return 1;
	}


/** CTENI ARGUMENTU  **/	
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
	/////printf("%s\n", read_str);
/*	char read_str[size_of_str];
	for (int a = 0; a < size_of_str; a++)
	{
		read_str[a] = pizza[a];
	}
*/	/////printf("%d\n", size_of_str);
////	printf("%s\n", read_str);
//	char read_str[] = *argv[1]
//	printf("%s", read_str);



/** CTENI SOUBORU SEZNAM.TXT **/
	int count = 0;
	int count2 = 0;
	char file_char;
	char first_char;
	//char arr_of_names[100];
	for (int i = 0; (first_char = getchar()) != EOF; i++)
	{
		char arr_of_names[100];
		char arr_of_phones[100];
		for (int h = 0; h < 100; h++)
		{
			arr_of_names[h] = '\0';
			arr_of_phones[h] = '\0';
		}
		arr_of_names[0] = first_char;
		for (int j = 1; (file_char = getchar()) != '\n'; j++)
		{
			arr_of_names[j] = file_char;
		}
		for (int k = 0; (file_char = getchar()) != '\n'; k++)
		{
			arr_of_phones[k] = file_char;
		}
	///	
	///
		for (int x = 0; arr_of_names[x] != '\0'; x++)
		{
			for (int y = 0; read_str[y] != '\0'; y++)
			{
				if (arr_of_names[x + y] == read_str[y])
				{
					count++;
					if (count == size_of_str)
					{
						break;
					}
				}
				else
				{
			//		x -= count + 1;
					count = 0;
				}
			}
			if (count == size_of_str)
			{
				break;
			}
		}
		//////Nahore pro jmeno, dole pro cislo
		for (int x = 0; arr_of_phones[x] != '\0'; x++)
		{
			for (int y = 0; read_str[y] != '\0'; y++)
			{
				if (arr_of_phones[x + y] == read_str[y])
				{
					count2++;
					if (count2 == size_of_str)
					{
						break;
					}
				}
				else
				{
			//		x -= count2 + 1;
					count2 = 0;
				}
			}
			if (count2 == size_of_str)
			{
				break;
			}
		}
		
		if (count == size_of_str || count2 == size_of_str)
		{
			printf("%s\n%s\n\n", arr_of_names, arr_of_phones);
		}
//*******//		printf("count: %d\ncount2: %d\nsize_of_str: %d\nread_str: %s\narr_of_names: %s\narr_of_phones: %s\n\n", count, count2, size_of_str, read_str, arr_of_names, arr_of_phones);
	///	
	/// Tady by se mela provest nejaka kontrola
	 
/***		printf("%s %d\n\n", arr_of_names, i);
		printf("%s %d\n\n", arr_of_phones, i);
***/	//	arr_of_names[j + 1] = '\0';
	}

/*
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
		//for (int j = 0; j < size_of_str; j++)
		//{
		//	if (read_str[j] == list_char)
		//	{
		//		count++;
		//	}
		//}
	}
*/




/////	printf("%s\n", read_str);
/*	if (count == size_of_str || count2 == size_of_str)
	{
		printf("Je tam %d\n", count);
	} 
*/	return 0;
}
