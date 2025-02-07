/**
 * @file code_generator.c
 * 
 * IFJ Projekt 2023
 * 
 * @author <xmicha94> Kryštof Michálek
*/

#include "code_generator.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>

/* Funkce pro generovani urciteho kodu v IFJcode23 */
void generate_code(parserData *data, int x, char *id, int i, int i2, double d, char *s) {
    switch (x)
    {
        case 0: // zacatek kodu
            printf(".IFJcode23\n\n");
            break;
        case 1: // func
            printf("LABEL %s\n", id);
            break;
        case 2: // LF var, LF let
            printf("DEFVAR LF@%s\n", id);
            break;
        case 3: // GF var, GF let
            printf("DEFVAR GF@%s\n", id);
            break;
        case 4: // move value int
            printf("MOVE %s int@%i\n", id, i);
            break;
        case 5: // move value double
            printf("MOVE %s float@%a\n", id, d);
            break;
        case 6: // move value string
            printf("MOVE %s string@%s\n", id, s);
            break;
        case 7: // +-*/ int
            if (d == 0.0) 
            {
                printf("ADD %s %s int@%i\n", id, id, i);
            }
            else if (d == 0.1)
            {
                printf("SUB %s %s int@%i\n", id, id, i);
            }
            else if (d == 0.2)
            {
                printf("MUL %s %s int@%i\n", id, id, i);
            }
            else if (d == 0.3)
            {
                printf("DIV %s %s int@%i\n", id, id, i);
            }
            break;  
        case 8: //TODOK expression - if podminka
            break;   
        case 9: // if (resp jump na konci)
            printf("JUMP %s%i\n", id, --data->nested_count_while);
            break;
        case 10: // write string
            printf("WRITE ");
            break;
        case 11: // write uvnitr
            if (strcmp(s, ""))
            {
                printf("%s", s);
                if (!strcmp(s, "string@"))
                {
                    for (int j = 0; id[j] != '\0'; j++)
                    {
                        i = (int)id[j];
                        if((i >= 0 && i <= 32) || i == 35 || i == 92) 
                        {
                            if (i == 92 && id[j+1] == 'n')
                            {
                                printf("\\010");
                                j++;
                            } 
                            else
                            {
                                printf("\\0%i", i);
                            }
                        }
                        else 
                        {
                            printf("%c", id[j]);
                        }
                    }
                    printf("\n");
                }
                else if (!strcmp(s, "int@"))
                {
                    printf("%i\n", i);
                }
                else if (!strcmp(s, "float@"))
                {
                    printf("%a\n", d);
                }
                else if (!strcmp(s, "GF@"))
                {
                    printf("%s\n", id);
                }
                else if (!strcmp(s, "LF@"))
                {
                    printf("%s\n", id);
                }
            }

            break;
        case 12: // move id id
            if (i == 1) 
            {
                printf("MOVE %s GF@%s\n", id, s);
            }
            else 
            {
                printf("MOVE %s LF@%s\n", id, s);
            }
            break;
        case 13: // +-*/ double
            if (i == 0) 
            {
                printf("ADD %s %s float@%a\n", id, id, d);
            }
            else if (i == 1)
            {
                printf("SUB %s %s float@%a\n", id, id, d);
            }
            else if (i == 2)
            {
                printf("MUL %s %s float@%a\n", id, id, d);
            }
            else if (i == 3)
            {
                printf("DIV %s %s float@%a\n", id, id, d);
            }
            break;
        case 14: // concat
            printf("CONCAT %s %s string@%s\n", id, id, s);
            break;
        case 15:
            if (i == 0)
            {
                if (d == 0.0) 
                {
                    printf("ADD %s %s GF@%s\n", id, id, s);
                }
                else if (d == 0.1)
                {
                    printf("SUB %s %s GF@%s\n", id, id, s);
                }
                else if (d == 0.2)
                {
                    printf("MUL %s %s GF@%s\n", id, id, s);
                }
                else if (d == 0.3)
                {
                    printf("DIV %s %s GF@%s\n", id, id, s);
                }
            }
            else 
            {
                if (d == 0.0) 
                {
                    printf("ADD %s %s LF@%s\n", id, id, s);
                }
                else if (d == 0.1)
                {
                    printf("SUB %s %s LF@%s\n", id, id, s);
                }
                else if (d == 0.2)
                {
                    printf("MUL %s %s LF@%s\n", id, id, s);
                }
                else if (d == 0.3)
                {
                    printf("DIV %s %s LF@%s\n", id, id, s);
                }
            }
            break;
        case 16: // else a while label
            if (!data->first_while) {
                printf("DEFVAR GF@MichalJeNasVudce\n");
                data->first_while = true;
            }
            printf("LABEL %s%i\n", id, data->nested_count_while);
            break;
        case 17: // < LT + true
            if (!data->first_while) {
                printf("DEFVAR GF@MichalJeNasVudce\n");
                data->first_while = true;
            }
            if(data->code_generator_token.type == INT) 
            {
                if (i == 0) 
                {
                    printf("LT GF@MichalJeNasVudce int@%i GF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("LT GF@MichalJeNasVudce int@%i LF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s); 
                }
                else
                {
                    printf("LT GF@MichalJeNasVudce int@%i int@%i\n", data->code_generator_token.attribute.i, i); 
                }    
            }
            else if (data->code_generator_token.type == DOUBLE)
            {
                if (i == 0) 
                {
                    printf("LT GF@MichalJeNasVudce float@%a GF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("LT GF@MichalJeNasVudce float@%a LF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else
                {
                    printf("LT GF@MichalJeNasVudce float@%a float@%a\n", data->code_generator_token.attribute.d, d);
                }   
            }
            else if (data->code_generator_token.type == STRING)
            {
                if (i == 0) 
                {
                    printf("LT GF@MichalJeNasVudce string@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("LT GF@MichalJeNasVudce string@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else
                {
                    printf("LT GF@MichalJeNasVudce string@%s string@%s\n", data->code_generator_token.attribute.s, s);
                }   
                
            }
            else if (data->code_generator_token.type == IDENTIFIER) 
            {
                if (data->currentToken.secondType == ID_INT)
                {
                    if (i2 == 0) 
                    {
                        printf("LT GF@MichalJeNasVudce GF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i);
                    }
                    else if (i2 == 1)
                    {
                        printf("LT GF@MichalJeNasVudce LF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i); 
                    }
                }
                else if (data->currentToken.secondType == ID_DOUBLE)
                {
                    if (i2 == 0) 
                    {
                        printf("LT GF@MichalJeNasVudce GF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d);
                    }
                    else if (i2 == 1)
                    {
                        printf("LT GF@MichalJeNasVudce LF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d); 
                    }
                }
                else if (data->currentToken.secondType == ID_STRING)
                {
                    if (i2 == 0) 
                    {
                        printf("LT GF@MichalJeNasVudce GF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                    }
                    else if (i2 == 1)
                    {
                        printf("LT GF@MichalJeNasVudce LF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                    }
                }
                else
                {
                    if (i2 == 0) 
                    {
                        if (i == 0) 
                        {
                            printf("LT GF@MichalJeNasVudce GF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                        }
                        else
                        {
                            printf("LT GF@MichalJeNasVudce GF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                    else if (i2 == 1)
                    {
                        if (i == 0) 
                        {
                            printf("LT GF@MichalJeNasVudce LF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                        else
                        {
                            printf("LT GF@MichalJeNasVudce LF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                }
            }
            printf("JUMPIFNEQ END%i GF@MichalJeNasVudce bool@true\n", data->nested_count_while++);
            break;
        case 18: // <= GT + false
            if (!data->first_while) {
                printf("DEFVAR GF@MichalJeNasVudce\n");
                data->first_while = true;
            }
            if(data->code_generator_token.type == INT) 
            {
                if (i == 0) 
                {
                    printf("GT GF@MichalJeNasVudce int@%i GF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("GT GF@MichalJeNasVudce int@%i LF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s); 
                }
                else
                {
                    printf("GT GF@MichalJeNasVudce int@%i int@%i\n", data->code_generator_token.attribute.i, i); 
                }    
            }
            else if (data->code_generator_token.type == DOUBLE)
            {
                if (i == 0) 
                {
                    printf("GT GF@MichalJeNasVudce float@%a GF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("GT GF@MichalJeNasVudce float@%a LF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else
                {
                    printf("GT GF@MichalJeNasVudce float@%a float@%a\n", data->code_generator_token.attribute.d, d);
                }   
            }
            else if (data->code_generator_token.type == STRING)
            {
                if (i == 0) 
                {
                    printf("GT GF@MichalJeNasVudce string@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("GT GF@MichalJeNasVudce string@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else
                {
                    printf("GT GF@MichalJeNasVudce string@%s string@%s\n", data->code_generator_token.attribute.s, s);
                }   
                
            }
            else if (data->code_generator_token.type == IDENTIFIER) 
            {
                if (data->currentToken.secondType == ID_INT)
                {
                    if (i2 == 0) 
                    {
                        printf("GT GF@MichalJeNasVudce GF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i);
                    }
                    else if (i2 == 1)
                    {
                        printf("GT GF@MichalJeNasVudce LF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i); 
                    }
                }
                else if (data->currentToken.secondType == ID_DOUBLE)
                {
                    if (i2 == 0) 
                    {
                        printf("GT GF@MichalJeNasVudce GF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d);
                    }
                    else if (i2 == 1)
                    {
                        printf("GT GF@MichalJeNasVudce LF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d); 
                    }
                }
                else if (data->currentToken.secondType == ID_STRING)
                {
                    if (i2 == 0) 
                    {
                        printf("GT GF@MichalJeNasVudce GF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                    }
                    else if (i2 == 1)
                    {
                        printf("GT GF@MichalJeNasVudce LF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                    }
                }
                else
                {
                    if (i2 == 0) 
                    {
                        if (i == 0) 
                        {
                            printf("GT GF@MichalJeNasVudce GF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                        }
                        else
                        {
                            printf("GT GF@MichalJeNasVudce GF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                    else if (i2 == 1)
                    {
                        if (i == 0) 
                        {
                            printf("GT GF@MichalJeNasVudce LF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                        else
                        {
                            printf("GT GF@MichalJeNasVudce LF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                }
            }
            printf("JUMPIEQ END%i GF@MichalJeNasVudce bool@true\n", data->nested_count_while++);
            break;
        case 19: // > GT + true
            if (!data->first_while) {
                printf("DEFVAR GF@MichalJeNasVudce\n");
                data->first_while = true;
            }
            if(data->code_generator_token.type == INT) 
            {
                if (i == 0) 
                {
                    printf("GT GF@MichalJeNasVudce int@%i GF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("GT GF@MichalJeNasVudce int@%i LF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s); 
                }
                else
                {
                    printf("GT GF@MichalJeNasVudce int@%i int@%i\n", data->code_generator_token.attribute.i, i); 
                }    
            }
            else if (data->code_generator_token.type == DOUBLE)
            {
                if (i == 0) 
                {
                    printf("GT GF@MichalJeNasVudce float@%a GF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("GT GF@MichalJeNasVudce float@%a LF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else
                {
                    printf("GT GF@MichalJeNasVudce float@%a float@%a\n", data->code_generator_token.attribute.d, d);
                }   
            }
            else if (data->code_generator_token.type == STRING)
            {
                if (i == 0) 
                {
                    printf("GT GF@MichalJeNasVudce string@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("GT GF@MichalJeNasVudce string@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else
                {
                    printf("GT GF@MichalJeNasVudce string@%s string@%s\n", data->code_generator_token.attribute.s, s);
                }   
                
            }
            else if (data->code_generator_token.type == IDENTIFIER) 
            {
                if (data->currentToken.secondType == ID_INT)
                {
                    if (i2 == 0) 
                    {
                        printf("GT GF@MichalJeNasVudce GF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i);
                    }
                    else if (i2 == 1)
                    {
                        printf("GT GF@MichalJeNasVudce LF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i); 
                    }
                }
                else if (data->currentToken.secondType == ID_DOUBLE)
                {
                    if (i2 == 0) 
                    {
                        printf("GT GF@MichalJeNasVudce GF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d);
                    }
                    else if (i2 == 1)
                    {
                        printf("GT GF@MichalJeNasVudce LF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d); 
                    }
                }
                else if (data->currentToken.secondType == ID_STRING)
                {
                    if (i2 == 0) 
                    {
                        printf("GT GF@MichalJeNasVudce GF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                    }
                    else if (i2 == 1)
                    {
                        printf("GT GF@MichalJeNasVudce LF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                    }
                }
                else
                {
                    if (i2 == 0) 
                    {
                        if (i == 0) 
                        {
                            printf("GT GF@MichalJeNasVudce GF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                        }
                        else
                        {
                            printf("GT GF@MichalJeNasVudce GF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                    else if (i2 == 1)
                    {
                        if (i == 0) 
                        {
                            printf("GT GF@MichalJeNasVudce LF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                        else
                        {
                            printf("GT GF@MichalJeNasVudce LF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                }
            }
            printf("JUMPINEQ END%i GF@MichalJeNasVudce bool@true\n", data->nested_count_while++);
            break;
        case 20: // >= LT + false
            if (!data->first_while) {
                printf("DEFVAR GF@MichalJeNasVudce\n");
                data->first_while = true;
            }
            if(data->code_generator_token.type == INT) 
            {
                if (i == 0) 
                {
                    printf("LT GF@MichalJeNasVudce int@%i GF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("LT GF@MichalJeNasVudce int@%i LF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s); 
                }
                else
                {
                    printf("LT GF@MichalJeNasVudce int@%i int@%i\n", data->code_generator_token.attribute.i, i); 
                }    
            }
            else if (data->code_generator_token.type == DOUBLE)
            {
                if (i == 0) 
                {
                    printf("LT GF@MichalJeNasVudce float@%a GF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("LT GF@MichalJeNasVudce float@%a LF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else
                {
                    printf("LT GF@MichalJeNasVudce float@%a float@%a\n", data->code_generator_token.attribute.d, d);
                }   
            }
            else if (data->code_generator_token.type == STRING)
            {
                if (i == 0) 
                {
                    printf("LT GF@MichalJeNasVudce string@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("LT GF@MichalJeNasVudce string@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else
                {
                    printf("LT GF@MichalJeNasVudce string@%s string@%s\n", data->code_generator_token.attribute.s, s);
                }   
                
            }
            else if (data->code_generator_token.type == IDENTIFIER) 
            {
                if (data->currentToken.secondType == ID_INT)
                {
                    if (i2 == 0) 
                    {
                        printf("LT GF@MichalJeNasVudce GF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i);
                    }
                    else if (i2 == 1)
                    {
                        printf("LT GF@MichalJeNasVudce LF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i); 
                    }
                }
                else if (data->currentToken.secondType == ID_DOUBLE)
                {
                    if (i2 == 0) 
                    {
                        printf("LT GF@MichalJeNasVudce GF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d);
                    }
                    else if (i2 == 1)
                    {
                        printf("LT GF@MichalJeNasVudce LF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d); 
                    }
                }
                else if (data->currentToken.secondType == ID_STRING)
                {
                    if (i2 == 0) 
                    {
                        printf("LT GF@MichalJeNasVudce GF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                    }
                    else if (i2 == 1)
                    {
                        printf("LT GF@MichalJeNasVudce LF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                    }
                }
                else
                {
                    if (i2 == 0) 
                    {
                        if (i == 0) 
                        {
                            printf("LT GF@MichalJeNasVudce GF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                        }
                        else
                        {
                            printf("LT GF@MichalJeNasVudce GF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                    else if (i2 == 1)
                    {
                        if (i == 0) 
                        {
                            printf("LT GF@MichalJeNasVudce LF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                        else
                        {
                            printf("LT GF@MichalJeNasVudce LF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                }
            }
            printf("JUMPIFEQ END%i GF@MichalJeNasVudce bool@true\n", data->nested_count_while++);
            break;
        case 21: // == EQ + false
            if (!data->first_while) {
                printf("DEFVAR GF@MichalJeNasVudce\n");
                data->first_while = true;
            }
            if(data->code_generator_token.type == INT) 
            {
                if (i == 0) 
                {
                    printf("EQ GF@MichalJeNasVudce int@%i GF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("EQ GF@MichalJeNasVudce int@%i LF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s); 
                }
                else
                {
                    printf("EQ GF@MichalJeNasVudce int@%i int@%i\n", data->code_generator_token.attribute.i, i); 
                }    
            }
            else if (data->code_generator_token.type == DOUBLE)
            {
                if (i == 0) 
                {
                    printf("EQ GF@MichalJeNasVudce float@%a GF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("EQ GF@MichalJeNasVudce float@%a LF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else
                {
                    printf("EQ GF@MichalJeNasVudce float@%a float@%a\n", data->code_generator_token.attribute.d, d);
                }   
            }
            else if (data->code_generator_token.type == STRING)
            {
                if (i == 0) 
                {
                    printf("EQ GF@MichalJeNasVudce string@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("EQ GF@MichalJeNasVudce string@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else
                {
                    printf("EQ GF@MichalJeNasVudce string@%s string@%s\n", data->code_generator_token.attribute.s, s);
                }   
                
            }
            else if (data->code_generator_token.type == IDENTIFIER) 
            {
                if (data->currentToken.secondType == ID_INT)
                {
                    if (i2 == 0) 
                    {
                        printf("EQ GF@MichalJeNasVudce GF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i);
                    }
                    else if (i2 == 1)
                    {
                        printf("EQ GF@MichalJeNasVudce LF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i); 
                    }
                }
                else if (data->currentToken.secondType == ID_DOUBLE)
                {
                    if (i2 == 0) 
                    {
                        printf("EQ GF@MichalJeNasVudce GF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d);
                    }
                    else if (i2 == 1)
                    {
                        printf("EQ GF@MichalJeNasVudce LF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d); 
                    }
                }
                else if (data->currentToken.secondType == ID_STRING)
                {
                    if (i2 == 0) 
                    {
                        printf("EQ GF@MichalJeNasVudce GF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                    }
                    else if (i2 == 1)
                    {
                        printf("EQ GF@MichalJeNasVudce LF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                    }
                }
                else
                {
                    if (i2 == 0) 
                    {
                        if (i == 0) 
                        {
                            printf("EQ GF@MichalJeNasVudce GF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                        }
                        else
                        {
                            printf("EQ GF@MichalJeNasVudce GF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                    else if (i2 == 1)
                    {
                        if (i == 0) 
                        {
                            printf("EQ GF@MichalJeNasVudce LF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                        else
                        {
                            printf("EQ GF@MichalJeNasVudce LF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                }
            }
            printf("JUMPIFNEQ END%i GF@MichalJeNasVudce bool@true\n", data->nested_count_while++);
            break;
        case 22:
            if (!data->first_while) {
                printf("DEFVAR GF@MichalJeNasVudce\n");
                data->first_while = true;
            }
            if(data->code_generator_token.type == INT) 
            {
                if (i == 0) 
                {
                    printf("EQ GF@MichalJeNasVudce int@%i GF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("EQ GF@MichalJeNasVudce int@%i LF@%s\n", data->code_generator_token.attribute.i, data->currentToken.attribute.s); 
                }
                else
                {
                    printf("EQ GF@MichalJeNasVudce int@%i int@%i\n", data->code_generator_token.attribute.i, i); 
                }    
            }
            else if (data->code_generator_token.type == DOUBLE)
            {
                if (i == 0) 
                {
                    printf("EQ GF@MichalJeNasVudce float@%a GF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("EQ GF@MichalJeNasVudce float@%a LF@%s\n", data->code_generator_token.attribute.d, data->currentToken.attribute.s);
                }
                else
                {
                    printf("EQ GF@MichalJeNasVudce float@%a float@%a\n", data->code_generator_token.attribute.d, d);
                }   
            }
            else if (data->code_generator_token.type == STRING)
            {
                if (i == 0) 
                {
                    printf("EQ GF@MichalJeNasVudce string@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else if (i == 1)
                {
                    printf("EQ GF@MichalJeNasVudce string@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                }
                else
                {
                    printf("EQ GF@MichalJeNasVudce string@%s string@%s\n", data->code_generator_token.attribute.s, s);
                }   
                
            }
            else if (data->code_generator_token.type == IDENTIFIER) 
            {
                if (data->currentToken.secondType == ID_INT)
                {
                    if (i2 == 0) 
                    {
                        printf("EQ GF@MichalJeNasVudce GF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i);
                    }
                    else if (i2 == 1)
                    {
                        printf("EQ GF@MichalJeNasVudce LF@%s int@%i\n", data->code_generator_token.attribute.s, data->currentToken.attribute.i); 
                    }
                }
                else if (data->currentToken.secondType == ID_DOUBLE)
                {
                    if (i2 == 0) 
                    {
                        printf("EQ GF@MichalJeNasVudce GF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d);
                    }
                    else if (i2 == 1)
                    {
                        printf("EQ GF@MichalJeNasVudce LF@%s float@%a\n", data->code_generator_token.attribute.s, data->currentToken.attribute.d); 
                    }
                }
                else if (data->currentToken.secondType == ID_STRING)
                {
                    if (i2 == 0) 
                    {
                        printf("EQ GF@MichalJeNasVudce GF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                    }
                    else if (i2 == 1)
                    {
                        printf("EQ GF@MichalJeNasVudce LF@%s string@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                    }
                }
                else
                {
                    if (i2 == 0) 
                    {
                        if (i == 0) 
                        {
                            printf("EQ GF@MichalJeNasVudce GF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s); 
                        }
                        else
                        {
                            printf("EQ GF@MichalJeNasVudce GF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                    else if (i2 == 1)
                    {
                        if (i == 0) 
                        {
                            printf("EQ GF@MichalJeNasVudce LF@%s GF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                        else
                        {
                            printf("EQ GF@MichalJeNasVudce LF@%s LF@%s\n", data->code_generator_token.attribute.s, data->currentToken.attribute.s);
                        }
                    }
                }
            }
            printf("JUMPIFEQ END%i GF@MichalJeNasVudce bool@true\n", data->nested_count_while++);
            break;
        case 23: // while jump na zacatek
            printf("JUMP WHILE%i\n", --data->nested_count_while);
            break;
        case 24:
            printf("LABEL %s%i\n", id, data->nested_count_while);
            break;
        default: 
            break;
    }
}
