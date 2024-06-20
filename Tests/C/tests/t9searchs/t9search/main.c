#include <stdio.h>

int main(/********int argc, char *argv[]********/)
{
    /********printf("%s\n", argv[1]);********/



    /**

    50 kontaktu, case INsensitive, 2 radky (max 100 znaku kazdy, 1. jmeno osoby, 2. telefon)


    hledani neprerusene posloupnosti
    1
    2 abc
    3 def
    4 ghi
    5 jkl
    6 mno
    7 pqrs
    8 tuv
    9 wxyz
    0 +


    kontakt nenalezen (Not found)
    /
    nalezeno 1 nebo vice kontaktu (format tisku: JMENO, TELCISLO), case INsensitive, poradi radku nezalezi


    ZAKAZ
    pouzivat funkce malloc a free
    pouzivat funkce fopen, fclose, fscanf
    pouzivat volani qsort, lsearch, bsearch, hsearch


    Chyby:
    neocekavana vstupni data (preruseni programu a strucna chybova hlaska na prislusny vystup)
    format vstupnich dat (preruseni programu a strucna chybova hlaska na prislusny vystup)
    volani funkce (preruseni programu a strucna chybova hlaska na prislusny vystup)

    **/


    //Jen pro pouziti v code:blocks
    char a1[] = "776522117";
    char a2[] = "845246862";
    char a3[] = "125159926";
    char a4[] = "196716359";
    char a5[] = "582703593";
    char a6[] = "752926832";
    char a7[] = "403723092";
    char a8[] = "501910204";
    char a9[] = "439835481";
    char a10[] = "225216695";

/*
    int j = 0;
    char hledane_cislo[100];
    scanf("%s", hledane_cislo);
    for (int i = 0; i < (sizeof(a1)/sizeof(char)); i++)
    {
        for (j = 0; j < (sizeof(hledane_cislo)/sizeof(char)); j++)
        {
            if (a1[i + j] == hledane_cislo[j])
            {
                if (j == (sizeof(hledane_cislo)/sizeof(char)))
                {
                    printf("Naslo se!\n");
                    break;
                }
            }
        }
    }

    if (j != (sizeof(hledane_cislo)/sizeof(char)))
    {
        printf("Nenaslo se!\n");
    }
*/


    /** funkci verze ze skoly **/
    char str2[100];
    int j = 0;
    scanf("%s", str2);
    int z;
    for (z = 0; str2[z] != '\0'; z++) //spocitam si velikost pole str2
    {
    }

    for (int i = 0; a1[i] != '\0'; i++)
    {
        if (j == z) //nesmim si smazat promennou j
        {
            break;
        }
        else if (str2[j] == a1[i]) //najdu prvni prvek v str2 a porovnavam
        {
            j++;
        }
        else //nejsou vsechny prvky za sebou
        {
            j = 0;
        }
    }

    //vypis
    if (j == z)
    {
        printf("je tam\n%s", a1);
    }
    else
    {
        printf("neni tam\n%s", a1);
    }



    return 0;
}
