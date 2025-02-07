/**
 * @file scanner.c
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
*/

#include "scanner.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/** Klicova slova */
char *keywords[KEYWORD_COUNT] = {"Double", "else", "func", "if", "Int", "let", "nil", "return", "String", "var", "while"};

/** Funkce zjisti, zda vysledny IDENTIFIER neni nahodou nejaky KEYWORD */
void isKeyword(Token *token)
{
    for (int i = 0; i < KEYWORD_COUNT; i++)
    {
        if (!strcmp(token->attribute.s, keywords[i]))
        {
            token->type = KEYWORD;
            return;
        }
    }
}

/** Funkce vytvori token pro parser, pripadne vraci cislo erroru */
int getToken(Token *token, int *lineCount)
{
    token->type = NO_TYPE;
    token->secondType = NO_SECOND_TYPE;
    token->attribute.d = 0;
    token->attribute.i = 0;
    token->attribute.s = "";
    
    char c;
    char *readString = (char *)calloc(255, sizeof(char));
    if (!readString)
    {
        return 150;
    }
    int readChar = 0;

    State state;
    state = START;

    int hexCount = 0;
    int commentCount = 0;
    while (true)
    {
        switch (state)
        {
            case START:
                c = getchar();
                if (c == '\r')
                {
                    break;
                }
                else if (c == '\n')
                {
                    state = EOL_STATE;
                }
                else if (c == EOF)
                {
                    state = EOF_STATE;
                }
                else if (c == '_')
                {
                    state = UNDERLINE_STATE;
                }
                else if (c == '"')
                {
                    state = STRING_START;
                }
                else if (c == '{')
                {
                    state = LEFT_CURLY_B_STATE;
                }
                else if (c == '}')
                {
                    state = RIGHT_CURLY_B_STATE;
                }
                else if (c == '/')
                {
                    state = DIVIDE_STATE;
                }
                else if (c == '*')
                {
                    state = MULTIPLY_STATE;
                }
                else if (c == '+')
                {
                    state = PLUS_STATE;
                }
                else if (c == '-')
                {
                    state = MINUS_STATE;
                }
                else if (c == '(')
                {
                    state = LEFT_BRACKET_STATE;
                }
                else if (c == ')')
                {
                    state = RIGHT_BRACKET_STATE;
                }
                else if (c == '!')
                {
                    state = NOT_NIL_STATE;
                }
                else if (c == '?')
                {
                    state = ALMOST_NIL_COND_STATE;
                }
                else if (c == '=')
                {
                    state = ASSIGNMENT_STATE;
                }
                else if (c == '>')
                {
                    state = GT_STATE;
                }
                else if (c == '<')
                {
                    state = LT_STATE;
                }
                else if (c == ',')
                {
                    state = COMMA_STATE;
                }
                else if (c == ':')
                {
                    state = COLON_STATE;
                }
                else if (c >= '0' && c <= '9')
                {
                    state = INT_STATE;
                }
                else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                {
                    state = ID_STATE;
                }
                else if (c == ' ' || c == '\t')
                {
                    break;
                }
                else // if (c == '%' || c == '#' ...)
                {
                    free(readString);
                    return 1;
                }

                if (c != '"') // string zapiseme do token->attribute.s bez uvozovek
                {
                    readString[readChar++] = c;
                }
                break;

            case EOL_STATE:
                (*lineCount)++;
                token->type = EOL_TYPE;
                token->attribute.s = "\n";
                free(readString);
                return 0;

            case EOF_STATE:
                token->type = EOF_TYPE;
                free(readString);
                return 0;

            case UNDERLINE_STATE:
                c = getchar();
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
                {
                    state = ID_STATE;
                }
                else
                {
                    token->type = UNDERLINE;
                    token->attribute.s = "_";
                    ungetc(c, stdin);
                    free(readString);
                    return 0;
                }
                readString[readChar++] = c;
                break;

            case COMMA_STATE:
                token->type = COMMA;
                token->attribute.s = ",";
                free(readString);
                return 0;

            case COLON_STATE:
                token->type = COLON;
                token->attribute.s = ":";
                free(readString);
                return 0;

            case ID_STATE:
                c = getchar();
                if (c == '?')
                {
                    //readString[readChar++] = c; Zde nebude ukladat znak, ale prida si ho az potom dalsi uzel
                    state = ID_QUESTIONMARK;
                }
                else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
                {
                    readString[readChar++] = c;
                }
                else
                {
                    ungetc(c, stdin);
                    token->type = IDENTIFIER;
                    token->attribute.s = readString;
                    isKeyword(token);
                    // free(readString); // musime vycistit az pri niceni tokenu
                    return 0;
                }
                break;
                
            case ID_QUESTIONMARK:
                token->type = IDENTIFIER;
                token->attribute.s = readString;
                isKeyword(token);
                if (token->type == KEYWORD && token->attribute.s[0] >= 'A' && token->attribute.s[0] <= 'Z')
                {
                    token->type = KEYWORD_NIL;
                    strcat(token->attribute.s, "?"); //? mby bude potrebovat neco jako readString[readChar++] = '?'; 
                    //free(readString);
                    return 0;
                }
                else
                {
                    ungetc('?', stdin);
                    //free(readString);
                    return 0;
                }
                break;

            case LEFT_CURLY_B_STATE:
                token->type = LEFT_CURLY_B;
                token->attribute.s = "{";
                free(readString);
                return 0;

            case RIGHT_CURLY_B_STATE:
                token->type = RIGHT_CURLY_B;
                token->attribute.s = "}";
                free(readString);
                return 0;

            case INT_STATE:
                c = getchar();
                if (c == '.')
                {
                    state = INT_DOT;
                }
                else if (c == 'e' || c == 'E')
                {
                    state = DOUBLE_E;
                    token->type = INT;
                    if (c == 'e')
                    {
                        token->secondType = LESS; // jakoze mensi e/E
                    }
                    else // if (c == 'E')
                    {
                        token->secondType = MORE; // jakoze vetsi e/E
                    }
                }
                else if (c >= '0' && c <= '9')
                {
                    // state = INT_STATE;
                }
                else
                {
                    token->type = INT;
                    token->attribute.i = atoi(readString); //? pravdopodobne kontrola mezi
                    ungetc(c, stdin);
                    free(readString);
                    return 0;
                }
                readString[readChar++] = c;
                break;
                
            case INT_DOT:
                c = getchar();
                if (c >= '0' && c <= '9')
                {
                    state = DOUBLE_STATE;
                }
                else
                {
                    ungetc(c, stdin);
                    free(readString);
                    return 1;
                }
                readString[readChar++] = c;
                break;
                
            case DOUBLE_STATE:
                c = getchar();
                if (c == 'e' || c == 'E')
                {
                    state = DOUBLE_E;
                    token->type = DOUBLE;
                    if (c == 'e')
                    {
                        token->secondType = LESS; // jakoze mensi e/E
                    }
                    else // if (c == 'E')
                    {
                        token->secondType = MORE; // jakoze vetsi e/E
                    }
                }
                else if (c >= '0' && c <= '9')
                {
                    // state = DOUBLE_STATE;
                    readString[readChar++] = c;
                }
                else
                {
                    token->type = DOUBLE;
                    token->attribute.d = atof(readString); //? pravdopodobne kontrola mezi
                    ungetc(c, stdin);
                    free(readString);
                    return 0;
                }
                break;
                
            case DOUBLE_E:
                c = getchar();
                if (c == '+' || c == '-')
                {
                    if (token->secondType == LESS) // e
                    {
                        readString[readChar++] = 'e';
                    }
                    else // if (token->secondType == MORE) // E
                    {
                        readString[readChar++] = 'E';
                    }
                    state = DOUBLE_E_SIGN;
                }
                else if (c >= '1' && c <= '9')
                {
                    if (token->secondType == LESS) // e
                    {
                        readString[readChar++] = 'e';
                    }
                    else // if (token->secondType == MORE) // E
                    {
                        readString[readChar++] = 'E';
                    }
                    state = DOUBLE_EXP_STATE;
                }
                else // if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') ale taky cokoliv jineho, protoze on stejne bude dal nacitat e/E... (resi dalsi token) 
                {
                    ungetc(c, stdin);
                    if (token->secondType == LESS) // e
                    {
                        ungetc('e', stdin);
                    }
                    else // if (token->secondType == MORE) // E
                    {
                        ungetc('E', stdin);
                    }
                    // token->type = INT/DOUBLE; zdedi z predchoziho
                    token->secondType = NO_SECOND_TYPE;
                    if (token->type == INT)
                    {
                        token->attribute.i = atoi(readString);
                    }
                    else // if (token->type == DOUBLE)
                    {
                        token->attribute.d = atof(readString);
                    }
                    free(readString);
                    return 0;
                }
                token->secondType = NO_SECOND_TYPE;
                readString[readChar++] = c;
                break;
                
            case DOUBLE_E_SIGN:
                c = getchar();
                if (c >= '1' && c <= '9')
                {
                    state = DOUBLE_EXP_STATE;
                }
                else
                {
                    ungetc(c, stdin);
                    free(readString);
                    return 1;
                }
                readString[readChar++] = c;
                break;
                
            case DOUBLE_EXP_STATE:
                c = getchar();
                if (c >= '0' && c <= '9')
                {
                    // state = DOUBLE_EXP_STATE;
                    readString[readChar++] = c;
                }
                else
                {
                    token->type = DOUBLE_EXPONENT;
                    token->attribute.d = atof(readString);
                    free(readString);
                    return 0;
                }
                break;
                
            case STRING_START:
                c = getchar();
                if (c == '\\')
                {
                    state = STRING_BACKSLASH;
                    readString[readChar++] = c;
                }
                else if (c == '"')
                {
                    state = STRING_STATE;
                }
                else if (c > 31 && c != '"' && c != '\\')
                {
                    // state = STRING_START;
                    readString[readChar++] = c;
                }
                else
                {
                    ungetc(c, stdin);
                    free(readString);
                    return 1;
                }
                break;
                
            case STRING_BACKSLASH:
                c = getchar();
                if (c == 'u')
                {
                    state = STRING_ESCAPE_U;
                }
                else if (c == '"' || c == 'n' || c == 'r' || c == 't' || c == '\\')
                {
                    state = STRING_START;
                }
                else
                {
                    free(readString);
                    return 1; // invalid escape sequence
                }
                readString[readChar++] = c;
                break;
                
            case STRING_ESCAPE_U:
                c = getchar();
                if (c == '{')
                {
                    hexCount = 0;
                    state = STRING_ESCAPE_U_BEGIN;
                }
                else
                {
                    ungetc(c, stdin);
                    free(readString);
                    return 1;
                }
                readString[readChar++] = c;
                break;
                
            case STRING_ESCAPE_U_BEGIN:
                c = getchar();
                if (hexCount < 8 && ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
                {
                    hexCount++;
                }
                else if (c == '}' && hexCount >= 1)
                {
                    state = STRING_START;
                }
                else
                {
                    ungetc(c, stdin);
                    free(readString);
                    return 1; // invalid \u{} sequence
                }
                readString[readChar++] = c;
                break;
                
            case STRING_STATE:
                c = getchar();
                if (c == '"' && readChar == 0)
                {
                    state = STRING_ALMOST_MORE_LINE;
                }
                else
                {
                    ungetc(c, stdin);
                    token->type = STRING;
                    token->attribute.s = readString;
                    return 0;
                }
                break;
                
            case STRING_ALMOST_MORE_LINE:
                c = getchar();
                if (c == ' ' || c == '\t' || c == '\r')
                {
                    // proste to preskoc
                }
                else if (c == '\n')
                {
                    (*lineCount)++;
                    state = STRING_MORE_LINE;
                }
                else
                {
                    free(readString);
                    return 1;
                }
                break;

            case STRING_MORE_LINE:
                c = getchar();
                if (c == '\r')
                {
                    break;
                }
                else if (c == '\n')
                {
                    (*lineCount)++;
                    state = STRING_THREE2END;
                }
                else
                {
                    // state = STRING_MORE_LINE;
                }
                readString[readChar++] = c;
                break;

            case STRING_THREE2END:
                c = getchar();
                if (c == '\r')
                {
                    break;
                }
                else if (c == '"')
                {
                    state = STRING_TWO2END;
                }
                else if (c == '\n')
                {
                    (*lineCount)++;
                    // state = STRING_THREE2END;
                }
                else if (c == ' ' || c == '\t')
                {
                    state = STRING_THREE2END;
                }
                else
                {
                    state = STRING_MORE_LINE;
                }
                readString[readChar++] = c;
                break;

            case STRING_TWO2END:
                c = getchar();
                if (c == '\r')
                {
                    break;
                }
                else if (c == '"')
                {
                    state = STRING_ONE2END;
                }
                else if (c == '\n')
                {
                    (*lineCount)++;
                    state = STRING_THREE2END;
                }
                else
                {
                    state = STRING_MORE_LINE;
                }
                readString[readChar++] = c;
                break;

            case STRING_ONE2END:
                c = getchar();
                if (c == '\r')
                {
                    break;
                }
                else if (c == '"')
                {
                    state = STRING_MORE_LINES_END;
                }
                else if (c == '\n')
                {
                    (*lineCount)++;
                    state = STRING_THREE2END;
                }
                else
                {
                    state = STRING_MORE_LINE;
                }
                readString[readChar++] = c;
                break;

            case STRING_MORE_LINES_END:
                for (int i = 0; i < 4; i++)
                {
                    readString[--readChar] = '\0'; // smaze EOL a 3 uvozovky ze stringu
                }
                token->type = MORE_LINES_STRING;
                token->attribute.s = readString;
                //free(readString);
                return 0;

            case LT_STATE:
                c = getchar();
                if (c == '=')
                {
                    state = LEQ_STATE;
                }
                else
                {
                    ungetc(c, stdin);
                    token->type = RELATION_COMPARE;
                    token->secondType = LESS;
                    token->attribute.s = "<";
                    free(readString);
                    return 0;
                }
                readString[readChar++] = c;
                break;

            case LEQ_STATE:
                token->type = RELATION_COMPARE;
                token->secondType = LESS_EQUAL;
                token->attribute.s = "<=";
                free(readString);
                return 0;

            case GT_STATE:
                c = getchar();
                if (c == '=')
                {
                    state = GEQ_STATE;
                }
                else
                {
                    ungetc(c, stdin);
                    token->type = RELATION_COMPARE;
                    token->secondType = MORE;
                    token->attribute.s = ">";
                    free(readString);
                    return 0;
                }
                readString[readChar++] = c;
                break;

            case GEQ_STATE:
                token->type = RELATION_COMPARE;
                token->secondType = MORE_EQUAL;
                token->attribute.s = ">=";
                free(readString);
                return 0;


            case MINUS_STATE:
                c = getchar();
                if (c == '>')
                {
                    state = ARROW_STATE;
                }
                else
                {
                    ungetc(c, stdin);
                    token->type = PLUS_MINUS;
                    token->secondType = MINUS;
                    token->attribute.s = "-";
                    free(readString);
                    return 0;
                }
                readString[readChar++] = c;
                break;

            case ARROW_STATE:
                token->type = ARROW;
                token->attribute.s = "->";
                free(readString);
                return 0;


            case PLUS_STATE:
                token->type = PLUS_MINUS;
                token->secondType = PLUS;
                token->attribute.s = "+";
                free(readString);
                return 0;

            case MULTIPLY_STATE:
                token->type = MULTIPLY_DIVIDE;
                token->secondType = MULTIPLY;
                token->attribute.s = "*";
                free(readString);
                return 0;

            case LEFT_BRACKET_STATE:
                token->type = LEFT_BRACKET;
                token->attribute.s = "(";
                free(readString);
                return 0;

            case RIGHT_BRACKET_STATE:
                token->type = RIGHT_BRACKET;
                token->attribute.s = ")";
                free(readString);
                return 0;

            case NOT_NIL_STATE:
                c = getchar();
                if (c == '=')
                {
                    state = NOT_EQ_STATE;
                }
                else
                {
                    ungetc(c, stdin);
                    token->type = NOT_NIL;
                    token->attribute.s = "!";
                    free(readString);
                    return 0;
                }
                readString[readChar++] = c;
                break;

            case NOT_EQ_STATE:
                token->type = RELATION_COMPARE;
                token->secondType = NOT_EQUAL;
                token->attribute.s = "!=";
                free(readString);
                return 0;

            case ALMOST_NIL_COND_STATE:
                c = getchar();
                if (c == '?')
                {
                    state = NIL_COND_STATE;
                }
                else
                {
                    ungetc(c, stdin);
                    free(readString);
                    return 1;
                }
                readString[readChar++] = c;
                break;

            case NIL_COND_STATE:
                token->type = NIL_CONDITION;
                token->attribute.s = "??";
                free(readString);
                return 0;

            case ASSIGNMENT_STATE:
                c = getchar();
                if (c == '=')
                {
                    state = EQ_STATE;
                }
                else
                {
                    ungetc(c, stdin);
                    token->type = ASSIGN;
                    token->attribute.s = "=";
                    free(readString);
                    return 0;
                }
                readString[readChar++] = c;
                break;

            case EQ_STATE:
                token->type = RELATION_COMPARE;
                token->secondType = EQUAL;
                token->attribute.s = "==";
                free(readString);
                return 0;

            case DIVIDE_STATE:
                c = getchar();
                if (c == '/' && commentCount == 0)
                {
                    state = LINE_COMMENT_START;
                }
                else if (c == '*')
                {
                    state = BLOCK_COMMENT_START;
                    commentCount++;
                }
                else if (commentCount == 0)
                {
                    ungetc(c, stdin);
                    token->type = MULTIPLY_DIVIDE;
                    token->secondType = DIVIDE;
                    token->attribute.s = "/";
                    free(readString);
                    return 0;
                }
                else
                {
                    ungetc(c, stdin);
                    state = BLOCK_COMMENT_START;
                }
                break;

            case LINE_COMMENT_START:
                c = getchar();
                if (c == '\n' || c == EOF)
                {
                    state = LINE_COMMENT_END;
                    ungetc(c, stdin);
                }
                break;

            case LINE_COMMENT_END:
                // token->type = LINE_COMMENT;
                // free(readString);
                // return 0;
                state = START;
                break;

            case BLOCK_COMMENT_START:
                c = getchar();
                if (c == '\n')
                {
                    (*lineCount)++;
                }
                if (c == '/')
                {
                    state = DIVIDE_STATE;
                }
                else if (c == '*')
                {
                    state = BLOCK_COMMENT_ENDING;
                }
                break;

            case BLOCK_COMMENT_ENDING:
                c = getchar();
                if (c == '/')
                {
                    commentCount--;
                    if (commentCount == 0)
                    {
                        state = BLOCK_COMMENT_END;
                    }
                    else
                    {
                        state = BLOCK_COMMENT_START;
                    }
                }
                else
                {
                    ungetc(c, stdin);
                    state = BLOCK_COMMENT_START;
                }
                break;

            case BLOCK_COMMENT_END:
                // token->type = BLOCK_COMMENT;
                // free(readString);
                // return 0;
                state = START;
                break;

            default:
                token->type = NO_TYPE;
                token->secondType = NO_SECOND_TYPE;
                token->attribute.d = 0;
                token->attribute.i = 0;
                token->attribute.s = "";
                free(readString);
                return 1;
        }
    }
    free(readString);
    return 1; // nedosazitelne
}