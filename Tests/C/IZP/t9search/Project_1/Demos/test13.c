#include <stdio.h>
#define MAX_ARR_SIZE 100

//JMENA PREVADI NA CISLA
char chars_to_numbers(char char_to_number)
{	
	char tmp_char;
	switch(char_to_number)
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
			tmp_char = ' ';
			break;
	}
	return tmp_char;
}


//HLEDA SUBSTRING V SEZNAMU
int substring(char str[], char sub_str[], int size_of_sub_str, int *count_of_found)
{
	int count = 0;
	for (int x = 0; str[x] != '\0'; x++)
	{
		for (int y = 0; sub_str[y] != '\0'; y++)
		{
			if (str[x + y] == sub_str[y] && str[x] == sub_str[0])
			{
				count++;
				if (count == size_of_sub_str)
				{
					break;
				}
			}
			else
			{
				count = 0;
			}
		}
		if (count == size_of_sub_str)
		{
			*count_of_found += 1;
			break;
		}
	}
	return count;
}

	


int main(int argc, char *argv[])
{	
		
/** KONTROLA, JESTLI MAME DOSTATECNY POCET ARGUMENTU **/
	if (argc < 2)
	{
		int lf_count = 0;
		char c;
		for (int i = 0; (c = getchar()) != EOF; i++)
		{
			if (c == '\n')
			{
				if (lf_count % 2 == 0)
				{
					c = ',';
					putchar(c);
					c = ' ';
				}
				lf_count++;
			}
			
			putchar(c);
		}
		return 0;
	}


/** CTENI ARGUMENTU DO POLE read_str **/
	int size_of_str = 0;
	char read_str[MAX_ARR_SIZE];
/*	for (int i = 0; i < MAX_ARR_SIZE; i++)
	{
		read_str[i] = '\0';
	}
*/	for (int i = 0; (argv[1][i]) != '\0'; i++)
	{
		if (!((argv[1][i] >= '0' && argv[1][i] <= '9') || argv[1][i] == '+'))
		{
			fprintf(stderr, "Expeted number as parameter.\nUsage: %s NUMBER_OR_'+' <file.txt\n", argv[0]);
			return 1;
		}
		read_str[i] = argv[1][i];
		size_of_str = i + 1;
	}
	read_str[size_of_str] = '\0';


/** CTENI SOUBORU, POROVNAVANI SE SUBSTRINGEM A VYPIS **/
	int count_of_found = 0;
	char file_char;
	char first_line_char;
	for (int i = 0; (first_line_char = getchar()) != EOF; i++)
	{
	

	/** VYTVORENI POLI PRO ULOZENI HODNOT ZE SOUBORU **/
		char arr_of_names[MAX_ARR_SIZE];
		char arr_of_phones[MAX_ARR_SIZE];
		char names_to_numbers[MAX_ARR_SIZE];
/*		for (int h = 0; h < MAX_ARR_SIZE; h++)
		{
			arr_of_names[h] = '\0';
			arr_of_phones[h] = '\0';
			names_to_numbers[h] = '\0';
		}
*/		
	
	/** VKLADANI HODNOT DO POLI ZE SOUBORU **/	
		arr_of_names[0] = first_line_char;
		for (int j = 1; (file_char = getchar()) != '\n'; j++)
		{
			if (j > MAX_ARR_SIZE)
			{
				fprintf(stderr, "There is line longer then 100 characters in file\n");
				return 1;
			}
			arr_of_names[j] = file_char;
		}
		arr_of_names[j + 1] = '\0'	
		
		for (int k = 0; (file_char = getchar()) != '\n'; k++)
		{
			if (k > MAX_ARR_SIZE)
			{
				fprintf(stderr, "There is line longer then 100 characters in file\n");
				return 1;
			}
			arr_of_phones[k] = file_char;
		}
		arr_of_phones[j + 1] = '\0';
	

	/** PREPIS arr_of_names NA CISLA A ULOZENI DO names_to_numbers **/
		for (int j = 0; arr_of_names[j] != '\0'; j++)
		{
			names_to_numbers[j] = chars_to_numbers(arr_of_names[j]);
			names_to_numbers[j + 1] = '\0';
		}
		names_to_numbers[j + 1]= '\0';


	/** POROVNAVANI RETEZCU A POCITANI SUBSTRINGU **/
		int count = substring(names_to_numbers, read_str, size_of_str, &count_of_found);
		int count2 = substring(arr_of_phones, read_str, size_of_str, &count_of_found);
			

	/** VYPIS JMEN A CISEL **/
		if (count == size_of_str || count2 == size_of_str)
		{
			printf("%s, %s\n", arr_of_names, arr_of_phones);
		}
	}

	
/** VYSLEDEK NENALEZEN **/
	if (count_of_found == 0)
	{
		printf("Not found\n");
	}		
 
	return 0;
}
