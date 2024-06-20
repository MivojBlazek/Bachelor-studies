#include <stdio.h>

int main(int argc, char *argv[])
{

// FUNGUJE BEZ OSETRENI RUZNYCH VYJIMEK


/** CISLA REPREZENTUJICI ZNAKY **/
/*
	char number1[] = {'1'};
	char number2[] = {'2', 'a', 'b', 'c'};
	char number3[] = {'3', 'd', 'e', 'f'};
	char number4[] = {'4', 'g', 'h', 'i'};
	char number5[] = {'5', 'j', 'k', 'l'};
	char number6[] = {'6', 'm', 'n', 'o'};
	char number7[] = {'7', 'p', 'q', 'r', 's'};	
	char number8[] = {'8', 't', 'u', 'v'};
	char number9[] = {'9', 'w', 'x', 'y', 'z'};
	char number0[] = {'0', '+'};
*/
	
	
/** KONTROLA JESTLI MAME DOSTATECNY POCET ARGUMENTU **/
/** TO DO **/
/** POKUD JE PROGRAM SPUSTENY BEZ ARGUMENTU, MA SE VYTISKNOUT SEZNAM.TXT CELY **/
	if (argc < 2)
	{
		fprintf(stderr, "ERROR\n");
		return 1;
	}


/** CTENI ARGUMENTU **/	
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
		char names_to_numbers[100];
		for (int h = 0; h < 100; h++)
		{
			arr_of_names[h] = '\0';
			arr_of_phones[h] = '\0';
			names_to_numbers[h]= '\0';
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

	/** PREPIS arr_of_names NA CISLA A ULOZENI DO NOVEHO POLE **/
		char tmp_char;
		for (int j = 0; arr_of_names[j] != '\0'; j++)
		{
			switch(arr_of_names[j])
			{
				case ' ':
					tmp_char = ' ';
					break;
				case '1':
					tmp_char = '1';
					break;
				case 'a' ... 'c':
				case 'A' ... 'C':
				case '2':
					tmp_char = '2';
					break;
				case 'd' ... 'f':
				case 'D' ... 'F':
				case '3':
					tmp_char = '3';
					break;
				case 'g' ... 'i':
				case 'G' ... 'I':
				case '4':
					tmp_char = '4';
					break;
				case 'j' ... 'l':
				case 'J' ... 'L':
				case '5':
					tmp_char = '5';
					break;
				case 'm' ... 'o':
				case 'M' ... 'O':
				case '6':
					tmp_char = '6';
					break;
				case 'p' ... 's':
				case 'P' ... 'S':
				case '7':
					tmp_char = '7';
					break;
				case 't' ... 'v':
				case 'T' ... 'V':
				case '8':
					tmp_char = '8';
					break;
				case 'w' ... 'z':
				case 'W' ... 'Z':
				case '9':
					tmp_char = '9';
					break;
				case '0':
				case '+':
					tmp_char = '0';
					break;
				default:
					//printf("Ve jmene je neplatny znak: %c\n", arr_of_names[j]);
					tmp_char = ' ';
					break;
			}
			names_to_numbers[j] = tmp_char;
		}
		//printf("%s\n", names_to_numbers);
	///	
	///
	/** ZACATEK POROVNAVANI RETEZCU **/
		for (int x = 0; names_to_numbers[x] != '\0'; x++)
		{
			for (int y = 0; read_str[y] != '\0'; y++)
			{
				if (names_to_numbers[x + y] == read_str[y] && names_to_numbers[x] == read_str[0])
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
				if (arr_of_phones[x + y] == read_str[y] && arr_of_phones[x] == read_str[0])
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
/*****/		printf("count: %d\ncount2: %d\nsize_of_str: %d\nread_str: %s\narr_of_names: %s\narr_of_phones: %s\nnames_to_numbers: %s\n\n", count, count2, size_of_str, read_str, arr_of_names, arr_of_phones, names_to_numbers);
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
