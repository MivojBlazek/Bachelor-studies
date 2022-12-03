#include <stdio.h>
#define MAX_ARR_SIZE 100

//PREPINAC MODU PRO -s PARAMETR
int mode;


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
	for (int y = 0; str[y] != '\0'; y++)
	{
		if (str[y] == sub_str[count])
		{
			count++;
			if (count == size_of_sub_str)
			{
				break;
			}
		}
		else
		{
			if (mode == 1)
			{
				y -= count;
				count = 0;
			}
		}
	}
	if (count == size_of_sub_str)
	{
		*count_of_found += 1;
	}
	return count;
}


// ULOZI JMENA A CISLA ZE SOUBORU DO POLI	
int array_from_file(char *array, char first_line_char)
{
	char file_char;
	int i = 0;
	
	if (first_line_char != '\0')
	{
		*array = first_line_char;
		i++;
	}
	else
	{
		for (int j = 0; j < MAX_ARR_SIZE; j++)
		{
			*(array + j) = '\0';
		}
	}
	while ((file_char = getchar()) != '\n')
	{
		if (i > MAX_ARR_SIZE)
		{
			return 1;
		}
		*(array + i) = file_char;
		*(array + i + 1) = '\0';
		i++;
	}
	return 0;
}


// CTENI ARGUMENTU DO POLE read_str
int read_func(char *read_str, int *size, char *argv)
{
	for (int i = 0; *(argv + i) != '\0'; i++)
	{
		if (i >= 99)
		{
			return 1;
		}
		else if (!((*(argv + i) >= '0' && *(argv + i) <= '9') || *(argv + i) == '+'))
		{
			return 2;
		}
		*(read_str + i) = *(argv + i);
		*size = i + 1;
	}
	*(read_str + *size) = '\0';
	return 0;
}



int main(int argc, char *argv[])
{
	mode = 2;

	
/** HLEDANI PARAMETRU -s **/
	if (argc > 1)
	{
		char s_str[] = "-s";
		for (int a = 0; (argv[1][a] != '\0') || (s_str[a] != '\0'); a++)
		{
			if (argv[1][a] != s_str[a])
			{
				mode = 1;
			}
		}
		if (mode != 1)
		{
			mode = 2;
		}
	}

	
/** VYPSANI CELEHO SOUBORU, POKUD NIKDO NEZADAL ZADNY ARGUMENT **/
	if (argc < 1 + mode)
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


/** UZIVATEL ZADAL PRILIS MOC PARAMETRU **/
	if (argc > 1 + mode)
	{
		fprintf(stderr, "Too much parameters.\nUsage: %s [-s] NUMBER_OR_'+' <file.txt\n", argv[0]);
		return 1;
	} 


/** CTENI ARGUMENTU DO POLE read_str **/
	int size_of_str = 0;
	char read_str[MAX_ARR_SIZE];
	int is_error = read_func(read_str, &size_of_str, argv[mode]);
	if (is_error == 1)
	{
		fprintf(stderr, "Cannot search contact with more than 100 characters\n");
		return 1;	
	}
	else if (is_error == 2)
	{
		fprintf(stderr, "Expeted number as parameter.\nUsage: %s [-s] NUMBER_OR_'+' <file.txt\n", argv[0]);
		return 1;
	}


/** CTENI SOUBORU, POROVNAVANI SE SUBSTRINGEM A VYPIS **/
	int count_of_found = 0;
	char first_line_char;
	for (int i = 0; (first_line_char = getchar()) != EOF; i++)
	{
	

	/** VYTVORENI POLI PRO ULOZENI HODNOT ZE SOUBORU **/
		char arr_of_names[MAX_ARR_SIZE];
		char arr_of_phones[MAX_ARR_SIZE];
		char names_to_numbers[MAX_ARR_SIZE];


	/** VKLADANI HODNOT DO POLI ZE SOUBORU **/
		if ((array_from_file(arr_of_names, first_line_char) == 1) || (array_from_file(arr_of_phones, first_line_char = '\0') == 1))
		{
			fprintf(stderr, "There is line longer then 100 characters in file on contact number: %d\n", (i + 1));
			return 1;
		}


	/** PREPIS arr_of_names NA CISLA A ULOZENI DO names_to_numbers **/
		for (int j = 0; arr_of_names[j] != '\0'; j++)
		{
			names_to_numbers[j] = chars_to_numbers(arr_of_names[j]);
			names_to_numbers[j + 1] = '\0';
		}


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
