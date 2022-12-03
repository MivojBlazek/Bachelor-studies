#include <stdio.h>

int main(int argc, char *argv[])
{
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
    char read_str[100];
    for (int t = 0; t < 100; t++)
    {
        read_str[t] = '\0';
    }
    for (int i = 0; (argv[1][i]) != '\0'; i++)
    {
        read_str[i] = argv[1][i];
        size_of_str = i + 1;
    }


/** CTENI SOUBORU SEZNAM.TXT **/
    int count = 0;
    int count2 = 0;
    char file_char;
    char first_char;
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
                    tmp_char = ' ';
                    break;
            }
            names_to_numbers[j] = tmp_char;
        }


    /** ZACATEK POROVNAVANI RETEZCU **/
    /** TO DO **/
    /** NENALEZEN KONTAKT -> "Not found" **/
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
                    count = 0;
                }
            }
            if (count == size_of_str)
            {
                break;
            }
        }

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
                    count2 = 0;
                }
            }
            if (count2 == size_of_str)
            {
                break;
            }
        }


    /** VYPIS JMEN A CISEL **/
    /** TO DO **/
    /** FORMAT VYPISU JE JINY -> "%s, %s\n" **/
        if (count == size_of_str || count2 == size_of_str)
        {
            printf("%s\n%s\n\n", arr_of_names, arr_of_phones);
        }
    }
    return 0;
}
