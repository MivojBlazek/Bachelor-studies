/**
 * @file parser.c
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
 * @author <xmicha94> Kryštof Michálek
*/

#include "expression.h"
#include "symtable.h"
#include "scanner.h"
#include "code_generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GET_TOKEN(token, line){                                 \
            errorNumber = getToken(&token, &line);              \
            if (errorNumber)                                    \
            {                                                   \
                if (errorNumber == 150)                         \
                {                                               \
                    return 99; /* scanner calloc failed */      \
                }                                               \
                return 1; /* scanner error */                   \
            }                                                   \
}

#define DONT_READ(){                                            \
            if (errorNumber == 0)                               \
            {                                                   \
                GET_TOKEN(data->currentToken, data->lineCount); \
            }                                                   \
            errorNumber = 0;                                    \
}

/** Globalni promenna zachytavajici errory */
int errorNumber;

/** Globalni kos na parametry */
Param voidParam;

//TODO expression.c:    kontrola datovych typu promennych v podminkach if a while
//TODO expression.c:    kdyz bude podminka: let xxx -> bude v else lokalni TS xxx s hodnotou nil
//TODO expression.c:    u vyrazu, kde je operator na konci radku, muzeme ocekavat operand na novym radku
//TODO parser.c:        volani funkce pred jejim definovani by melo projit, ale parser vyhodnoti jako chybu
//TODO parser.c:        return nemusi byt ve vsech vetvich kodu, takze se muze stat, ze funkce nic nevrati
//TODO parser.c:        kontrola datovych typu, poradi a dalsich veci u parametru pri volani funkce
//TODO                  veskera prace s nil

//? block() bude vzdy jen globalni
//? statement() zase nikdy nebude globalni

/** Funkce, ktera prida vestavene funkce do globalni tabulky symbolu */
void addInternalFunc(Node **tree)
{
    Node newNode[10] = {
        {.name = "readString",  .const_let = false, .func = true, .type = "String?", .params = malloc(1 * sizeof(Param))},
        {.name = "readInt",     .const_let = false, .func = true, .type = "Int?",    .params = malloc(1 * sizeof(Param))},
        {.name = "readDouble",  .const_let = false, .func = true, .type = "Double?", .params = malloc(1 * sizeof(Param))},
        {.name = "write",       .const_let = false, .func = true, .type = "",        .params = malloc(1 * sizeof(Param))},
        {.name = "Int2Double",  .const_let = false, .func = true, .type = "Double",  .params = malloc(2 * sizeof(Param))},
        {.name = "Double2Int",  .const_let = false, .func = true, .type = "Int",     .params = malloc(2 * sizeof(Param))},
        {.name = "length",      .const_let = false, .func = true, .type = "Int",     .params = malloc(2 * sizeof(Param))},
        {.name = "substring",   .const_let = false, .func = true, .type = "String?", .params = malloc(4 * sizeof(Param))},
        {.name = "ord",         .const_let = false, .func = true, .type = "Int",     .params = malloc(2 * sizeof(Param))},
        {.name = "chr",         .const_let = false, .func = true, .type = "String",  .params = malloc(2 * sizeof(Param))}
    };

    newNode[0].params[0].name = NULL;

    newNode[1].params[0].name = NULL;

    newNode[2].params[0].name = NULL;

    newNode[3].params[0].name = NULL;

    newNode[4].params[1].name = NULL;
    newNode[4].params[0].name = "term";
    newNode[4].params[0].callingName = "_";
    newNode[4].params[0].type = "Int";

    newNode[5].params[1].name = NULL;
    newNode[5].params[0].name = "term";
    newNode[5].params[0].callingName = "_";
    newNode[5].params[0].type = "Double";

    newNode[6].params[1].name = NULL;
    newNode[6].params[0].name = "s";
    newNode[6].params[0].callingName = "_";
    newNode[6].params[0].type = "String";

    newNode[7].params[3].name = NULL;
    newNode[7].params[0].name = "s";
    newNode[7].params[0].callingName = "of";
    newNode[7].params[0].type = "String";
    newNode[7].params[1].name = "i";
    newNode[7].params[1].callingName = "startingAt";
    newNode[7].params[1].type = "Int";
    newNode[7].params[2].name = "j";
    newNode[7].params[2].callingName = "endingBefore";
    newNode[7].params[2].type = "Int";

    newNode[8].params[1].name = NULL;
    newNode[8].params[0].name = "c";
    newNode[8].params[0].callingName = "_";
    newNode[8].params[0].type = "String";

    newNode[9].params[1].name = NULL;
    newNode[9].params[0].name = "i";
    newNode[9].params[0].callingName = "_";
    newNode[9].params[0].type = "Int";

    for (int i = 0; i < 10; i++)
    {
        insertNode(tree, &(newNode[i]));
    }
}

/** Hlavni funkce prekladace */
int startCompiling()
{
    // Initialization
    errorNumber = 0;
    parserData *data = (parserData *)malloc(sizeof(parserData));
    if (!data)
    {
        return 99; //! malloc failed
    }
    data->stack = (tokenStack *)malloc(sizeof(tokenStack));
    if (!data->stack)
    {
        return 99; //! malloc failed
    }
    generate_code(data,  0, "", 0, 0, 0.0, "");
    tStack_Init(data->stack);
    data->tsCount = -1;
    initTree(&(data->symTable[++(data->tsCount)])); // globalni TS

    data->paramCount = 0;
    data->returning = false;
    data->lineCount = 1;
    data->flag = false;
    data->nested_count_else = 0;
    data->nested_count_while = 0;
    data->first_while = false;

    // Main code
    addInternalFunc(&(data->symTable[data->tsCount]));
    GET_TOKEN(data->currentToken, data->lineCount);
    int result = prog(data);
    // fprintf(stderr, "Radek %d: %d\n", data->lineCount, result); //! DEBUG

    // Memory cleaner
    disposeTree(&(data->symTable[(data->tsCount)--])); // globalni TS
    // tStack_Dispose(data->stack); //? if (data->stack) tStack_Dispose(data->stack);
    free(data->stack);
    free(data);
    return result;
}

/** <PROG> */
int prog(parserData *data)
{
    if (data->currentToken.type == EOL_TYPE || !strcmp(data->currentToken.attribute.s, "func") || !strcmp(data->currentToken.attribute.s, "let") || !strcmp(data->currentToken.attribute.s, "var") || data->currentToken.type == IDENTIFIER || !strcmp(data->currentToken.attribute.s, "while") || !strcmp(data->currentToken.attribute.s, "if")) // pravidlo 1
    {
        if ((errorNumber = block(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        return prog(data);
    }
    else if (data->currentToken.type == EOF_TYPE) // pravidlo 2
    {
        return 0; //! Syntakticka analyza prosla v poradku
    }
    return 2; //! pokud neni na tento typ pravidlo
}

/** <BLOCK> */
int block(parserData *data)
{
    if (data->currentToken.type == EOL_TYPE) // pravidlo 3
    {
        return 0;
    }
    else if (!strcmp(data->currentToken.attribute.s, "func")) // pravidlo 4
    {
        Node tmpNode;
        tmpNode.const_let = false;
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != IDENTIFIER)
        {
            return 2; //! chybi nazev funkce
        }
        tmpNode.name = data->currentToken.attribute.s;
        tmpNode.func = true;
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != LEFT_BRACKET)
        {
            return 2; //! chybi otevirajici zavorka u funkce
        }
        data->paramCount = 0;
        initTree(&(data->symTable[++(data->tsCount)]));
        GET_TOKEN(data->currentToken, data->lineCount);

        tmpNode.params = malloc(255 * sizeof(Param));

        if ((errorNumber = param(data, &tmpNode)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != RIGHT_BRACKET)
        {
            return 2; //! chybi uzavirajici zavorka u funkce
        }
        data->returning = false;
        data->expected_return = "";
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = returning(data, &tmpNode)) > 0)
        {
            return errorNumber;
        }

        Node *searching = searchNode(data->symTable[0], tmpNode.name); // zjistovani, zda funkce uz neni definovana
        if (searching != NULL)
        {
            if (!searching->func)
            {
                Node *tmp = searchNode(searching->left, tmpNode.name);
                if (tmp == NULL)
                {
                    tmp = searchNode(searching->right, tmpNode.name);
                }

                if (tmp != NULL)
                {
                    return 9; //! uz existuje takova funkce
                }
                // continue
            }
            else
            {
                return 9; //! uz existuje takova funkce
            }
        }

        insertNode(&(data->symTable[0]), &tmpNode); // pridani funkce do GTS
        DONT_READ();
        if ((errorNumber = eol(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != LEFT_CURLY_B)
        {
            return 2; //! chybi otevirajici slozena zavorka
        }
        generate_code(data, 1, tmpNode.name, 0, 0, 0.0, "");
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = st_list(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != RIGHT_CURLY_B)
        {
            return 2; //! chybi uzavirajici slozena zavorka
        }
        disposeTree(&(data->symTable[(data->tsCount)--]));
        return 0;
    }
    else if (!strcmp(data->currentToken.attribute.s, "let")) // pravidlo 5
    {
        Node tmpNode;
        tmpNode.const_let = true;
        tmpNode.func = false;
        tmpNode.params = malloc(255 * sizeof(Param)); //? xd
        tmpNode.params[0].name = NULL;
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != IDENTIFIER)
        {
            return 2; //! chybi nazev promenne
        }
        tmpNode.name = data->currentToken.attribute.s;
        generate_code(data, 3, tmpNode.name, 0, 0, 0.0, "");
        strcpy(data->idOfVar, "GF@");
        strcat(data->idOfVar, tmpNode.name);
        GET_TOKEN(data->currentToken, data->lineCount);
        tmpNode.type = "";
        if ((errorNumber = with_type(data, &tmpNode)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        data->value_type = "";
        if ((errorNumber = assign_mark(data)) > 0)
        {
            return errorNumber;
        }
        errorNumber = 0;
        if (!strcmp(tmpNode.type, ""))
        {
            if (!strcmp(data->value_type, ""))
            {
                return 2; //! nelze odovodit typ a nemame ani typ
            }
            tmpNode.type = data->value_type; // odvodi typ z hodnoty
        }
        else
        {
            if (strcmp(data->value_type, ""))
            {
                if (strncmp(tmpNode.type, data->value_type, 3))
                {
                    if (!((!strncmp(tmpNode.type, "Double", 3) && !strncmp(data->value_type, "Double", 3)) || (!strncmp(tmpNode.type, "Int", 3) && !strncmp(data->value_type, "Int", 3))))
                    {
                        return 7; //! odvozeny typ neni stejny jako prirazeny
                    }
                }
            }
        }

        Node *searching = searchNode(data->symTable[data->tsCount], tmpNode.name);
        if (searching != NULL)
        {
            if (searching->func)
            {
                Node *tmp = searchNode(searching->left, tmpNode.name);
                if (tmp == NULL)
                {
                    tmp = searchNode(searching->right, tmpNode.name);
                }

                if (tmp != NULL)
                {
                    return 3; //! uz existuje takova globalni promenna
                }
                // continue
            }
            else
            {
                return 3; //! uz existuje takova globlani promenna
            }
        }

        insertNode(&(data->symTable[data->tsCount]), &tmpNode);
        return errorNumber;
    }
    else if (!strcmp(data->currentToken.attribute.s, "var")) // pravidlo 6
    {
        Node tmpNode;
        tmpNode.const_let = false;
        tmpNode.func = false;
        tmpNode.params = malloc(255 * sizeof(Param)); //? xd
        tmpNode.params[0].name = NULL;
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != IDENTIFIER)
        {
            return 2; //! chybi nazev promenne
        }
        tmpNode.name = data->currentToken.attribute.s;
        generate_code(data, 3, tmpNode.name, 0, 0, 0.0,  ""); // GF
        strcpy(data->idOfVar, "GF@");
        strcat(data->idOfVar, tmpNode.name);
        //data->idOfVar = tmpNode.name;
        GET_TOKEN(data->currentToken, data->lineCount);
        tmpNode.type = "";
        if ((errorNumber = with_type(data, &tmpNode)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        data->value_type = "";
        if ((errorNumber = assign_mark(data)) > 0)
        {
            return errorNumber;
        }
        errorNumber = 0;
        if (!strcmp(tmpNode.type, ""))
        {
            if (!strcmp(data->value_type, ""))
            {
                return 2; //! nelze odovodit typ a nemame ani typ
            }
            tmpNode.type = data->value_type; // odvodi typ z hodnoty
        }
        else
        {
            if (strcmp(data->value_type, ""))
            {
                if (strncmp(tmpNode.type, data->value_type, 3))
                {
                    if (!((!strncmp(tmpNode.type, "Double", 3) && !strncmp(data->value_type, "Double", 3)) || (!strncmp(tmpNode.type, "Int", 3) && !strncmp(data->value_type, "Int", 3))))
                    {
                        return 7; //! odvozeny typ neni stejny jako prirazeny
                    }
                }
            }
        }

        Node *searching = searchNode(data->symTable[data->tsCount], tmpNode.name);
        if (searching != NULL)
        {
            if (searching->func)
            {
                Node *tmp = searchNode(searching->left, tmpNode.name);
                if (tmp == NULL)
                {
                    tmp = searchNode(searching->right, tmpNode.name);
                }

                if (tmp != NULL)
                {
                    return 3; //! uz existuje takova globalni promenna
                }
                // continue
            }
            else
            {
                return 3; //! uz existuje takova globlani promenna
            }
        }

        insertNode(&(data->symTable[data->tsCount]), &tmpNode);
        return errorNumber;
    }
    else if (data->currentToken.type == IDENTIFIER) // pravidlo 7
    {
        Token funcName = data->currentToken;
        GET_TOKEN(data->currentToken, data->lineCount);
        strcpy(data->idOfVar, "GF@");
        strcat(data->idOfVar, funcName.attribute.s);
        if (data->currentToken.type == LEFT_BRACKET)
        {
            int ts;
            for (ts = data->tsCount; ts >= 0; ts--)
            {
                Node *searching = searchNode(data->symTable[data->tsCount], funcName.attribute.s);
                if (searching != NULL)
                {
                    if (!searching->func)
                    {
                        Node *tmp = searchNode(searching->left, funcName.attribute.s);
                        if (tmp == NULL)
                        {
                            tmp = searchNode(searching->right, funcName.attribute.s);
                        }

                        if (tmp != NULL)
                        {
                            // funkce existuje
                            break;
                        }
                        // continue
                    }
                    else
                    {
                        // funkce existuje
                        break;
                    }
                }
            }
            if (ts == -1)
            {
                return 3; //! funkce neexistuje
            }
        }
        else if (data->currentToken.type == ASSIGN)
        {
            int ts;
            for (ts = data->tsCount; ts >= 0; ts--)
            {
                Node *searching = searchNode(data->symTable[data->tsCount], funcName.attribute.s);
                if (searching != NULL)
                {
                    if (searching->func)
                    {
                        Node *tmp = searchNode(searching->left, funcName.attribute.s);
                        if (tmp == NULL)
                        {
                            tmp = searchNode(searching->right, funcName.attribute.s);
                        }

                        if (tmp != NULL)
                        {
                            // promenna existuje
                            if (searching->const_let)
                            {
                                return 3; //! nelze menit hodnotu konstanty
                            }
                            break;
                        }
                        // continue
                    }
                    else
                    {
                        // promenna existuje
                        if (searching->const_let)
                        {
                            return 3; //! nelze menit hodnotu konstanty
                        }
                        break;
                    }
                }
            }
            if (ts == -1)
            {
                return 5; //! promenna neexistuje
            }
        }
        errorNumber = void_or_assign(data, &funcName);
        return errorNumber;
    }
    else if (!strcmp(data->currentToken.attribute.s, "while")) // pravidlo 8
    {
        initTree(&(data->symTable[++(data->tsCount)]));
        generate_code(data, 16, "WHILE", data->nested_count_while, 0, 0.0, ""); //TODOK dodelat podminku a jumpy
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = expression(data)) > 0)
        {
            return errorNumber;
        }

        if ((errorNumber = eol(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != LEFT_CURLY_B)
        {
            return 2; //! chybi otevirajici slozena zavorka
        }
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = st_list(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != RIGHT_CURLY_B)
        {
            return 2; //! chybi uzavirajici slozena zavorka
        }
        generate_code(data, 23, "", 0, 0, 0.0, "");
        generate_code(data, 24, "END", data->nested_count_while, 0, 0.0, "");
        disposeTree(&(data->symTable[(data->tsCount)--]));
        return 0;
    }
    else if (!strcmp(data->currentToken.attribute.s, "if")) // pravidlo 9
    {
        //data->nested_count_while++;
        initTree(&(data->symTable[++(data->tsCount)]));
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = expression(data)) > 0) 
        {
            return errorNumber;
        }

        if ((errorNumber = eol(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != LEFT_CURLY_B)
        {
            return 2; //! chybi otevirajici slozena zavorka
        }
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = st_list(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != RIGHT_CURLY_B)
        {
            return 2; //! chybi uzavirajici slozena zavorka
        }
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = eol(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        generate_code(data, 9, "END", data->nested_count_while, 0, 0.0, "");
        disposeTree(&(data->symTable[(data->tsCount)--]));
        return elsing(data);
    }
    return 2; //! pokud neexistuje zadny validni block
}

/** <VOID_OR_ASSIGN> */
int void_or_assign(parserData *data, Token *funcName)
{
    // TODO nekde by se melo kontrolovat, jestli funcName je v TS
    if (!strcmp(funcName->attribute.s, "write")) {
        generate_code(data, 10, "", 0, 0, 0.0, "");
    }
    if (data->currentToken.type == LEFT_BRACKET) // pravidlo 10
    {
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = calling_param(data, funcName)) > 0)
        {
            return errorNumber;
        }

        if (data->currentToken.type != RIGHT_BRACKET)
        {
            return 2; //! chybi uzavirajici zavorka
        }
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != EOL_TYPE && data->currentToken.type != EOF_TYPE)
        {
            return value(data, E_TOKEN);
        }
        if (data->currentToken.type == EOF_TYPE)
        {
            return -1;
        }
        return 0;
    }
    else if (data->currentToken.type == ASSIGN) // pravidlo 11
    {
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = eol(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        return assignment(data);
    }
    return 2;
}

/** <ELSE> */
int elsing(parserData *data)
{
    if (!strcmp(data->currentToken.attribute.s, "else")) // pravidlo 12
    {
        generate_code(data, 16, "ELSE", data->nested_count_while, 0, 0.0, "");
        initTree(&(data->symTable[++(data->tsCount)]));
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = eol(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != LEFT_CURLY_B)
        {
            return 2; //! chybi uzavirajici zavorka
        }
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = st_list(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != RIGHT_CURLY_B)
        {
            return 2; //! chybi uzavirajici zavorka
        }
        generate_code(data, 24, "END", data->nested_count_while, 0, 0.0, "");
        disposeTree(&(data->symTable[(data->tsCount)--]));
        return 0;
    }
    return 2; // pravidlo 13: return -1; //! jelikoz v zakladni verzi zadani else musi byt povinne
}

/** <EOL> */
int eol(parserData *data)
{
    if (data->currentToken.type == EOL_TYPE) // pravidlo 13
    {
        return 0;
    }
    return -1; // pravidlo 14
}

/** <PARAM> */
int param(parserData *data, Node *nodeToChange)
{
    Node tmpNode;
    tmpNode.func = false;
    tmpNode.const_let = false;
    tmpNode.params = malloc(255 * sizeof(Param)); //? xd
    tmpNode.params[0].name = NULL;
    Param tmpParam;
    if (data->currentToken.type == IDENTIFIER) // pravidlo 15
    {
        tmpParam.callingName = data->currentToken.attribute.s;
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != IDENTIFIER && data->currentToken.type != UNDERLINE)
        {
            return 2; //! chybi nazev parametru
        }
        else if (data->currentToken.type == UNDERLINE) // ID _ : type
        {
            tmpParam.name = "_";
            GET_TOKEN(data->currentToken, data->lineCount);
            if (data->currentToken.type != COLON)
            {
                return 2; //! chybi : (nevime typ)
            }
            GET_TOKEN(data->currentToken, data->lineCount);
            if ((errorNumber = data_type(data, &tmpParam)) != 0)
            {
                return errorNumber;
            }
        }
        else // ID ID : type
        {
            tmpParam.name = data->currentToken.attribute.s;
            GET_TOKEN(data->currentToken, data->lineCount);
            if (data->currentToken.type != COLON)
            {
                return 2; //! chybi : (nevime typ)
            }
            GET_TOKEN(data->currentToken, data->lineCount);
            if ((errorNumber = data_type(data, &tmpParam)) != 0)
            {
                return errorNumber;
            }
        }
        nodeToChange->params[data->paramCount] = tmpParam;
        tmpNode.name = tmpParam.name;
        tmpNode.type = tmpParam.type;
        insertNode(&(data->symTable[data->tsCount]), &tmpNode);
        if (changing)
        {
            return 3; //! redefinice promenne v parametru
        }
        data->paramCount++;
        GET_TOKEN(data->currentToken, data->lineCount);
        return next_param(data, nodeToChange);
    }
    else if (data->currentToken.type == UNDERLINE) // pravidlo 16
    {
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != IDENTIFIER)
        {
            return 2; //! chybi nazev parametru
        }
        tmpParam.callingName = "_";
        tmpParam.name = data->currentToken.attribute.s;
        // _ ID : type
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != COLON)
        {
            return 2; //! chybi : (nevime typ)
        }
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = data_type(data, &tmpParam)) != 0)
        {
            return errorNumber;
        }
        nodeToChange->params[data->paramCount] = tmpParam;
        tmpNode.name = tmpParam.name;
        tmpNode.type = tmpParam.type;
        insertNode(&(data->symTable[data->tsCount]), &tmpNode);
        if (changing)
        {
            return 3; //! redefinice promenne v parametru
        }
        data->paramCount++;
        GET_TOKEN(data->currentToken, data->lineCount);
        return next_param(data, nodeToChange);
    }
    return -1; // pravidlo 17
}

/** <NEXT_PARAM> */
int next_param(parserData *data, Node *nodeToChange)
{
    if (data->currentToken.type == COMMA) // pravidlo 18
    {
        GET_TOKEN(data->currentToken, data->lineCount);
        return param(data, nodeToChange);
    }
    return -1; // pravidlo 19
}

/** <RETURN> */
int returning(parserData *data, Node *nodeToChange)
{
    if (data->currentToken.type == ARROW) // pravidlo 20
    {
        data->returning = true;
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = data_type(data, &voidParam)) != 0)
        {
            return errorNumber;
        }
        nodeToChange->type = voidParam.type;
        data->expected_return = voidParam.type;
        return 0;
    }
    return -1; // pravidlo 21
}

/** <ST_LIST> */
int st_list(parserData *data)
{
    if (!strcmp(data->currentToken.attribute.s, "return") || data->currentToken.type == EOL_TYPE || !strcmp(data->currentToken.attribute.s, "let") || !strcmp(data->currentToken.attribute.s, "var") || data->currentToken.type == IDENTIFIER || !strcmp(data->currentToken.attribute.s, "while") || !strcmp(data->currentToken.attribute.s, "if")) // pravidlo 22
    {
        if ((errorNumber = statement(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        return st_list(data);
    }
    return -1; // pravidlo 23
}

/** <STATEMENT> */
int statement(parserData *data)
{
    if (!strcmp(data->currentToken.attribute.s, "return")) // pravidlo 24
    {
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type == EOL_TYPE && !(data->returning))
        {
            GET_TOKEN(data->currentToken, data->lineCount);
            return 0;
        }
        else if (data->currentToken.type == RIGHT_CURLY_B && !(data->returning))
        {
            return -1;
        }
        else if (data->returning)
        {
            if ((errorNumber = value(data, EOL_TYPE)) > 0)
            {
                return errorNumber;
            }

            if (strncmp(data->value_type, data->expected_return, 3))
            {
                return 4; //! neplatny typ navratove hodnoty oproti navratove hodnote funkce
            }
            return -1;
        }
        return 4; //! neplatny token za return ve funkci bez navratove hodnoty
    }
    else if (!strcmp(data->currentToken.attribute.s, "if")) // pravidlo 25
    {
        //data->nested_count_while++;
        initTree(&(data->symTable[++(data->tsCount)]));
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = expression(data)) > 0)
        {
            return errorNumber;
        }

        if ((errorNumber = eol(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != LEFT_CURLY_B)
        {
            return 2; //! chybi otevirajici slozena zavorka
        }
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = st_list(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != RIGHT_CURLY_B)
        {
            return 2; //! chybi uzavirajici slozena zavorka
        }
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = eol(data)) > 0)
        {
            return errorNumber;
        }
        generate_code(data, 9, "END", data->nested_count_while, 0, 0.0, "");
        disposeTree(&(data->symTable[(data->tsCount)--]));
        DONT_READ();
        return elsing(data);
    }
    else if (!strcmp(data->currentToken.attribute.s, "let")) // pravidlo 26
    {
        Node tmpNode;
        tmpNode.const_let = true;
        tmpNode.func = false;
        tmpNode.params = malloc(255 * sizeof(Param)); //? xd
        tmpNode.params[0].name = NULL;
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != IDENTIFIER)
        {
            return 2; //! chybi nazev promenne
        }
        tmpNode.name = data->currentToken.attribute.s;
        GET_TOKEN(data->currentToken, data->lineCount);
        tmpNode.type = "";
        if ((errorNumber = with_type(data, &tmpNode)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        data->value_type = "";
        if ((errorNumber = assign_mark(data)) > 0)
        {
            return errorNumber;
        }
        errorNumber = 0;

        if (!strcmp(tmpNode.type, ""))
        {
            if (!strcmp(data->value_type, ""))
            {
                return 2; //! nelze odovodit typ a nemame ani typ
            }
            tmpNode.type = data->value_type; // odvodi typ z hodnoty
        }
        else
        {
            if (strcmp(data->value_type, ""))
            {
                if (strncmp(tmpNode.type, data->value_type, 3))
                {
                    if (!((!strncmp(tmpNode.type, "Double", 3) && !strncmp(data->value_type, "Double", 3)) || (!strncmp(tmpNode.type, "Int", 3) && !strncmp(data->value_type, "Int", 3))))
                    {
                        return 7; //! odvozeny typ neni stejny jako prirazeny
                    }
                }
            }
        }
        if (searchNode(data->symTable[data->tsCount], tmpNode.name))
        {
            return 3; //! uz existuje takova promenna
        }
        insertNode(&(data->symTable[data->tsCount]), &tmpNode);

        return errorNumber;
    }
    else if (!strcmp(data->currentToken.attribute.s, "var")) // pravidlo 27
    {
        Node tmpNode;
        tmpNode.const_let = false;
        tmpNode.func = false;
        tmpNode.params = malloc(255 * sizeof(Param)); //? xd
        tmpNode.params[0].name = NULL;
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != IDENTIFIER)
        {
            return 2; //! chybi nazev promenne
        }
        tmpNode.name = data->currentToken.attribute.s;
        generate_code(data, 2, tmpNode.name, 0, 0, 0.0, "");
        strcpy(data->idOfVar, "LF@");
        strcat(data->idOfVar, tmpNode.name);
        GET_TOKEN(data->currentToken, data->lineCount);
        tmpNode.type = "";
        if ((errorNumber = with_type(data, &tmpNode)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        data->value_type = "";
        if ((errorNumber = assign_mark(data)) > 0)
        {
            return errorNumber;
        }
        errorNumber = 0;

        if (!strcmp(tmpNode.type, ""))
        {
            if (!strcmp(data->value_type, ""))
            {
                return 2; //! nelze odovodit typ a nemame ani typ
            }
            tmpNode.type = data->value_type; // odvodi typ z hodnoty
        }
        else
        {
            if (strcmp(data->value_type, ""))
            {
                if (strncmp(tmpNode.type, data->value_type, 3))
                {
                    if (!((!strncmp(tmpNode.type, "Double", 3) && !strncmp(data->value_type, "Double", 3)) || (!strncmp(tmpNode.type, "Int", 3) && !strncmp(data->value_type, "Int", 3))))
                    {
                        return 7; //! odvozeny typ neni stejny jako prirazeny
                    }
                }
            }
        }
        if (searchNode(data->symTable[data->tsCount], tmpNode.name))
        {
            return 3; //! uz existuje takova promenna
        }
        insertNode(&(data->symTable[data->tsCount]), &tmpNode);
        return errorNumber;
    }
    else if (data->currentToken.type == IDENTIFIER) // pravidlo 28
    {     
        Token funcName = data->currentToken;
        Node *searching = searchNode(data->symTable[0], funcName.attribute.s);
        if (searching == NULL) 
        {
            strcpy(data->idOfVar, "LF@");
        }
        else
        {
            strcpy(data->idOfVar, "GF@");
        }
        strcat(data->idOfVar, funcName.attribute.s);
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = void_or_assign(data, &funcName)) > 0)
        {
            return errorNumber;
        }
        errorNumber = 0;
        return errorNumber;
    }
    else if (!strcmp(data->currentToken.attribute.s, "while")) // pravidlo 29
    {
        initTree(&(data->symTable[++(data->tsCount)]));
        generate_code(data, 16, "WHILE", data->nested_count_while, 0, 0.0, "");
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = expression(data)) > 0)
        {
            return errorNumber;
        }

        if ((errorNumber = eol(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != LEFT_CURLY_B)
        {
            return 2; //! chybi otevirajici slozena zavorka
        }
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = st_list(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != RIGHT_CURLY_B)
        {
            return 2; //! chybi uzavirajici slozena zavorka
        }
        generate_code(data, 23, "", 0, 0, 0.0, "");
        generate_code(data, 24, "END", data->nested_count_while, 0, 0.0, "");
        disposeTree(&(data->symTable[(data->tsCount)--]));
        return 0;
    }
    else if (data->currentToken.type == EOL_TYPE) // pravidlo 30
    {
        return 0;
    }
    return 2;
}

/** <DATA_TYPE> */
int data_type(parserData *data, Param *paramToSave)
{
    if (!strcmp(data->currentToken.attribute.s, "Int") || !strcmp(data->currentToken.attribute.s, "Int?")) // pravidlo 31
    {
        paramToSave->type = data->currentToken.attribute.s;
        return 0;
    }
    else if (!strcmp(data->currentToken.attribute.s, "Double") || !strcmp(data->currentToken.attribute.s, "Double?")) // pravidlo 32
    {
        paramToSave->type = data->currentToken.attribute.s;
        return 0;
    }
    else if (!strcmp(data->currentToken.attribute.s, "String") || !strcmp(data->currentToken.attribute.s, "String?")) // pravidlo 33
    {
        paramToSave->type = data->currentToken.attribute.s;
        return 0;
    }
    return 8; //! chybi/neplatny typ
}

/** <ASSIGN_MARK> */
int assign_mark(parserData *data)
{
    if (data->currentToken.type == ASSIGN) // pravidlo 34
    {
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = eol(data)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        return assignment(data);
    }
    return -1; // pravidlo 35
}

/** <ASSIGNMENT> */
int assignment(parserData *data)
{
    if (data->currentToken.type == IDENTIFIER) // pravidlo 36
    {
        tStack_Init(data->stack); //? proc musim znova initnout
        tStack_Push(data->stack, data->currentToken);
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != LEFT_BRACKET)
        {
            Token tmp;
            tStack_Top(data->stack, &tmp);
            Node *searching = searchNode(data->symTable[0], tmp.attribute.s);
            if (searching == NULL) 
            {
                generate_code(data, 12, data->idOfVar, 0, 0, 0.0, tmp.attribute.s);
            }
            else
            {
                generate_code(data, 12, data->idOfVar, 1, 0, 0.0, tmp.attribute.s);
            }

            //! chybi otevirajici zavorka -> je to vyraz
            if ((errorNumber = value(data, EOL_TYPE)) > 0)
            {
                // move a x treba
                return errorNumber;
            }
            return -1;
        }
        Token funcName;
        tStack_Pop(data->stack, &funcName);

        int ts = data->tsCount;
        Node *searching = NULL;
        while (ts >= 0)
        {
            searching = searchNode(data->symTable[ts--], funcName.attribute.s);
            if (searching)
            {
                ts++;
                data->value_type = searching->type;
                break;
            }
        }
        if (ts == -1)
        {
            return 3; //! v tabulce symbolu toto neni (funkce)
        }

        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = calling_param(data, &funcName)) > 0)
        {
            return errorNumber;
        }
        DONT_READ();
        if (data->currentToken.type != RIGHT_BRACKET)
        {
            return 2; //! chybi uzavirajici zavorka
        }

        data->value_type = searching->type;
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type != EOL_TYPE)
        {
            return value(data, E_TOKEN);
        }
        return -1;
    }
    else if (data->currentToken.type == LEFT_BRACKET || data->currentToken.type == INT || data->currentToken.type == DOUBLE || data->currentToken.type == DOUBLE_EXPONENT || data->currentToken.type == STRING || data->currentToken.type == MORE_LINES_STRING) // pravidlo 37
    {
        //TODO switch na INT, DOUBLE, STRING
        if (data->currentToken.type == INT) 
        {
            generate_code(data, 4, data->idOfVar, data->currentToken.attribute.i, 0, 0.0, "");
        }
        else if (data->currentToken.type == DOUBLE || data->currentToken.type == DOUBLE_EXPONENT)
        {
            generate_code(data, 5, data->idOfVar, 0, 0, data->currentToken.attribute.d, "");
        }
        else if (data->currentToken.type == STRING || data->currentToken.type == MORE_LINES_STRING) 
        {
            generate_code(data, 6, data->idOfVar, 0, 0, 0.0, data->currentToken.attribute.s);
        }
        return value(data, EOL_TYPE);
    }
    return 2;
}

/** <CALLING_PARAM> */
int calling_param(parserData *data, Token *funcName)
{
    if (data->currentToken.type == LEFT_BRACKET || data->currentToken.type == INT || data->currentToken.type == DOUBLE || data->currentToken.type == DOUBLE_EXPONENT || data->currentToken.type == STRING || data->currentToken.type == MORE_LINES_STRING) // pravidlo 38
    {
        if (data->currentToken.type == STRING || data->currentToken.type == MORE_LINES_STRING)
        {
            generate_code(data, 11, data->currentToken.attribute.s, 0, 0, 0.0, "string@"); // tisknuti normalniho stringu napr. write("Hello world!")
        }
        else if (data->currentToken.type == INT) 
        {
            generate_code(data, 11, "", data->currentToken.attribute.i, 0, 0.0, "int@");
        }
        else if (data->currentToken.type == DOUBLE || data->currentToken.type == DOUBLE_EXPONENT) 
        {
            generate_code(data, 11, "", 0, 0, data->currentToken.attribute.d, "float@");
        }

        if ((errorNumber = value(data, COMMA)) > 0)
        {
            return errorNumber;
        }

        return next_calling(data, funcName);
    }
    else if (data->currentToken.type == IDENTIFIER) // pravidlo 39 // TODO kontrola promenne, co se pouziva jako volajici parametr funkce (jestli promenna existuje, jestli sedi typ s funkci (funkce ma ve funcName jmeno))
    {
        Node *searching = searchNode(data->symTable[0], data->currentToken.attribute.s);
        if (searching == NULL) 
        {
            strcpy(data->idOfVar, "LF@");
        }
        else
        {
            strcpy(data->idOfVar, "GF@");
        }
        generate_code(data, 11, data->currentToken.attribute.s, 0, 0, 0.0, data->idOfVar); // vypisuje identifiery napr write(b)
        Token voidToken;
        tStack_Init(data->stack); //? proc musim znova initnout
        tStack_Push(data->stack, data->currentToken);
        GET_TOKEN(data->currentToken, data->lineCount);
        if (data->currentToken.type == COLON)
        {
            tStack_Pop(data->stack, &voidToken);
            if ((errorNumber = with_value(data)) > 0)
            {
                return errorNumber;
            }
        }
        else if (data->currentToken.type == RIGHT_BRACKET)
        {
            tStack_Pop(data->stack, &voidToken);
            return -1;
        }
        else
        {
            if ((errorNumber = value(data, COMMA)) > 0)
            {
                return errorNumber;
            }
        }
        
        return next_calling(data, funcName);
    }
    return -1; // pravidlo 40
}

/** <WITH_VALUE> */
int with_value(parserData *data)
{
    if (data->currentToken.type == COLON) // pravidlo 41
    {
        GET_TOKEN(data->currentToken, data->lineCount);
        return value(data, COMMA);
    }
    return -1; // pravidlo 42
}

/** <NEXT_CALLING> */
int next_calling(parserData *data, Token *funcName)
{
    if (data->currentToken.type == COMMA) // pravidlo 43
    {
        GET_TOKEN(data->currentToken, data->lineCount);
        generate_code(data, 10, "", 0, 0, 0.0, "");
        return calling_param(data, funcName);
    }
    return -1; // pravidlo 44
}

/** <VALUE> */
int value(parserData *data, tokenType expectedType) // pravidla 45, 46, 47
{
    char *dataType[7] = {"", "Int", "Int?", "Double", "Double?", "String", "String?"};
    int selectType = 0;
    if ((errorNumber = valueExpression(data, expectedType, &selectType)) > 0)
    {
        return errorNumber;
    }
    data->value_type = dataType[selectType];
    return 0;
}

/** <WITH_TYPE> */
int with_type(parserData *data, Node *nodeToChange)
{
    if (data->currentToken.type == COLON) // pravidlo 48
    {
        GET_TOKEN(data->currentToken, data->lineCount);
        if ((errorNumber = data_type(data, &voidParam)) > 0)
        {
            return errorNumber;
        }
        nodeToChange->type = voidParam.type;
        return errorNumber;
    }
    return -1; // pravidlo 49
}
