/**
 * @file expression.c
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
 * @author <xmicha94> Kryštof Michálek
*/

#include "expression.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "code_generator.h"
#include "parser.h"

#define GET_TOKEN_EXPR(token, line){                            \
            errorNumberExpr = getToken(&token, &line);          \
            if (errorNumberExpr)                                \
            {                                                   \
                if (errorNumberExpr == 150)                     \
                {                                               \
                    return 99; /* scanner calloc failed */      \
                }                                               \
                return 1; /* scanner error */                   \
            }                                                   \
}

/** Globalni promenna zachytavajici errory */
int errorNumberExpr = 0;

/** Uchovava informaci o tom, jestli dany vyraz muze byt podminkou (jestli byl pouzity relacni operator) */
bool relation;

/** Precedencni tabulka */
precedenceTableSign precedenceTable[9][9] = {
//   !   ??  ==,!=,<,>,<=,>=   +,-   *,/   (   )    i   $
    {UN, RG,       RG,         RG,   RG,  UN, RG,  UN, RG}, // !
    {LF, LF,       LF,         LF,   LF,  LF, RG,  LF, RG}, // ??
    {LF, RG,       UN,         LF,   LF,  LF, RG,  LF, RG}, // ==,!=,<,>,<=,>=
    {LF, RG,       RG,         RG,   LF,  LF, RG,  LF, RG}, // +,-
    {LF, RG,       RG,         RG,   RG,  LF, RG,  LF, RG}, // *,/
    {LF, RG,       LF,         LF,   LF,  LF, EQU, LF, UN}, // (
    {RG, RG,       RG,         RG,   RG,  UN, RG,  UN, RG}, // )
    {RG, RG,       RG,         RG,   RG,  UN, RG,  UN, RG}, // i
    {LF, LF,       LF,         LF,   LF,  LF, UN,  LF, UN}  // $
};

/** Funkce prevede tokeny na pravidla k nim odpovidajici */
precedenceTableRulesStruct token2Rules(Token token)
{
    precedenceTableRulesStruct newRule;
    newRule.size = 4; // <E+E, <E*E, <E??E, ...
    switch (token.type)
    {
        case NOT_NIL:
            newRule.rule = E_NOT_NIL;
            newRule.size = 3;   // <E!
            break;

        case NIL_CONDITION:
            newRule.rule = E_NIL_CONDITION_E;
            break;

        case RELATION_COMPARE:
            relation = true;
            if (token.secondType == EQUAL)
            {
                newRule.rule = E_EQUAL_E;
            }
            else if (token.secondType == NOT_EQUAL)
            {
                newRule.rule = E_NOT_EQUAL_E;
            }
            else if (token.secondType == LESS)
            {
                newRule.rule = E_LESS_E;
            }
            else if (token.secondType == MORE)
            {
                newRule.rule = E_MORE_E;
            }
            else if (token.secondType == LESS_EQUAL)
            {
                newRule.rule = E_LESS_EQUAL_E;
            }
            else // if (token.secondType == MORE_EQUAL)
            {
                newRule.rule = E_MORE_EQUAL_E;
            }
            break;

        case PLUS_MINUS:
            if (token.secondType == PLUS)
            {
                newRule.rule = E_PLUS_E;
            }
            else
            {
                newRule.rule = E_MINUS_E;
            }
            break;

        case MULTIPLY_DIVIDE:
            if (token.secondType == MULTIPLY)
            {
                newRule.rule = E_MULTIPLY_E;
            }
            else
            {
                newRule.rule = E_DIVIDE_E;
            }
            break;

        case LEFT_BRACKET:
            newRule.rule = LEFTPAR_E_RIGHTPAR; //! pravdepodobne to nepujde, protoze ?? maji nepovinny operand nalevo (tady predpokladam, ze za zavorkou bude vzdy i nebo E)
            break;

        case RIGHT_BRACKET:
            newRule.rule = LEFTPAR_E_RIGHTPAR;
            break;

        case INT:
        case DOUBLE:
        case DOUBLE_EXPONENT:
        case STRING:
        case MORE_LINES_STRING:
        case IDENTIFIER:
            newRule.rule = OPERAND;
            newRule.size = 2;
            break;
        
        default:
            newRule.rule = NO_RULE;
            newRule.size = 0;
            break;
    }
    return newRule;
}

/** Funkce najde datovy typ ID v TS a vrati ho */
int findTypeOfID(parserData *data)
{
    Node *searching = NULL;

    int ts = data->tsCount;
    while (ts >= 0)
    {
        searching = searchNode(data->symTable[ts--], data->currentToken.attribute.s);
        if (searching)
        {
            ts++;
            break;
        }
    }
    if (ts == -1)
    {
        return 85; //! promenna neexistuje
    }
    else
    {
        if (!(searching->func))
        {
            // nasel jsem ji
            if (!strcmp(searching->type, "Int"))
            {
                return ID_INT;
            }
            else if (!strcmp(searching->type, "Int?"))
            {
                return ID_INT;
            }
            else if (!strcmp(searching->type, "Double"))
            {
                return ID_DOUBLE;
            }
            else if (!strcmp(searching->type, "Double?"))
            {
                return ID_DOUBLE;
            }
            else if (!strcmp(searching->type, "String"))
            {
                return ID_STRING;
            }
            else if (!strcmp(searching->type, "String?"))
            {
                return ID_STRING;
            }
            else
            {
                return 87; //! promenna nema typ
            }
        }
        else
        {
            // nasel jsem funkci, zkusim jeste hledat
            Node *tmp = searchNode(searching->left, data->currentToken.attribute.s);
            if (tmp == NULL)
            {
                tmp = searchNode(searching->right, data->currentToken.attribute.s);
            }

            if (tmp == NULL)
            {
                return 85; //! promenna neexistuje
            }
            else
            {
                // nasel jsem ji
                if (!strcmp(searching->type, "Int"))
                {
                    return ID_INT;
                }
                else if (!strcmp(searching->type, "Int?"))
                {
                    return ID_INT;
                }
                else if (!strcmp(searching->type, "Double"))
                {
                    return ID_DOUBLE;
                }
                else if (!strcmp(searching->type, "Double?"))
                {
                    return ID_DOUBLE;
                }
                else if (!strcmp(searching->type, "String"))
                {
                    return ID_STRING;
                }
                else if (!strcmp(searching->type, "String?"))
                {
                    return ID_STRING;
                }
                else
                {
                    return 87; //! promenna nema typ
                }
            }
        }
    }
}

/** Hlavni funkce pro zpracovavani vyrazu do {. Vraci error_number nebo 0 */
int expression(parserData *data) // TODO typ v podmince mimo podminky: let IDENTIFIER
{
    relation = false;
    Token dollar;
    dollar.type = DOLLAR;
    dollar.attribute.s = "$";
    tStack_Dispose(data->stack);
    tStack_Init(data->stack);
    tStack_Push(data->stack, dollar);

    Token top;
    int Xth_from_top = 0;
    bool defined = false;
    if (data->currentToken.type == LEFT_CURLY_B)
    {
        return 2; //! chybi jakakoliv podminka
    }
    //data->code_generator_token = data->currentToken;
    do
    {
        if (data->currentToken.type == EOF_TYPE)
        {
            data->currentToken.type = DOLLAR;
        }

        if (defined)
        {
            // minuly token byl let
            Node tmpNode;
            tmpNode.func = false;
            tmpNode.name = data->currentToken.attribute.s;
            tmpNode.params = malloc(1 * sizeof(Param));
            tmpNode.params[0].name = NULL;
            tmpNode.const_let = true;
            tmpNode.type = "";

            Node *searching = NULL;
            int ts = data->tsCount;
            while (ts >= 0)
            {
                searching = searchNode(data->symTable[ts--], data->currentToken.attribute.s);
                if (searching)
                {
                    ts++;
                    if (!strcmp(searching->type, "Int?"))
                    {
                        tmpNode.type = "Int";
                    }
                    else if (!strcmp(searching->type, "Double?"))
                    {
                        tmpNode.type = "Double";
                    }
                    else if (!strcmp(searching->type, "String?"))
                    {
                        tmpNode.type = "String";
                    }
                    else
                    {
                        tmpNode.type = searching->type;
                    }
                    break;
                }
            }
            if (ts == -1)
            {
                return 5; //! promenna neexistuje
            }

            insertNode(&(data->symTable[data->tsCount]), &tmpNode);
            defined = false;
        }


        tStack_Top(data->stack, &top);
        Xth_from_top = 0;
        if (top.type == E_TOKEN)
        {
            tStack_Second_Top(data->stack, &top);
            Xth_from_top = 1;
        }
        bool priznak = false;
        bool priznak1 = false;

        if (data->currentToken.type == INT)
        {
            data->currentToken.secondType = ID_INT;
            data->currentToken.type = IDENTIFIER;
        }
        else if (data->currentToken.type == DOUBLE || data->currentToken.type == DOUBLE_EXPONENT)
        {
            data->currentToken.secondType = ID_DOUBLE;
            data->currentToken.type = IDENTIFIER;
        }
        else if (data->currentToken.type == STRING || data->currentToken.type == MORE_LINES_STRING)
        {
            data->currentToken.secondType = ID_STRING;
            data->currentToken.type = IDENTIFIER;
        }
        else if (data->currentToken.type == KEYWORD && !strcmp(data->currentToken.attribute.s, "let"))
        {
            // redefinice promenne v podmince if nebo while
            defined = true;
            relation = true;
            GET_TOKEN_EXPR(data->currentToken, data->lineCount);
            continue;
        }
        else if (data->currentToken.type == EOL_TYPE)
        {
            GET_TOKEN_EXPR(data->currentToken, data->lineCount);
            if (data->currentToken.type == LEFT_CURLY_B)
            {
                data->currentToken.type = DOLLAR;
            }
            continue;
        }
        else if (data->currentToken.type == IDENTIFIER)
        {
            priznak = true;
            int typeID = findTypeOfID(data);
            if (typeID >= 80)
            {
                return typeID - 80;
            }
            data->currentToken.secondType = typeID;
        }
        
        if (data->code_generator_token.type == IDENTIFIER)
        {
            priznak1 = true; 
        }

        if (precedenceTable[top.type][data->currentToken.type] == LF)
        {
            int GF_LF_choosing = -1; 
            int GF_LF_choosing1 = -1; 
            if (priznak) 
            {
                Node *searching = searchNode(data->symTable[0], data->currentToken.attribute.s);
                if (searching == NULL) 
                {
                    GF_LF_choosing = 1;
                }
                else
                {
                    GF_LF_choosing = 0;
                }
            }

            if (priznak1) 
            {
                Node *searching = searchNode(data->symTable[0], data->code_generator_token.attribute.s);
                if (searching == NULL) 
                {
                    GF_LF_choosing1 = 1;
                }
                else
                {
                    GF_LF_choosing1 = 0;
                }
            }

            //printf("%i %i\n", priznak, priznak1);
            if (data->flag) 
            {
                if (!strcmp(data->operator, "<"))
                {
                    if (priznak && priznak1) 
                    {
                        generate_code(data, 17, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                        priznak1 = false;
                    }
                    else if (priznak)
                    {
                        generate_code(data, 17, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                    }
                    else if (priznak1)
                    {
                        generate_code(data, 17, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak1 = false;
                    }
                    else if (data->currentToken.secondType == ID_INT && data->code_generator_token.type == INT)
                    {
                        generate_code(data, 17, "", data->currentToken.attribute.i, 0, 0.0, "");
                    }
                    else if (data->currentToken.secondType == ID_DOUBLE && data->code_generator_token.type == DOUBLE)
                    {
                        generate_code(data, 17, "", 0, 0, data->currentToken.attribute.d, "");
                    }
                    else if (data->currentToken.secondType == ID_STRING && data->code_generator_token.type == STRING)
                    {
                        generate_code(data, 17, "", 0, 0, 0.0, data->currentToken.attribute.s);
                    }
                }
                else if (!strcmp(data->operator, "<="))
                {
                    if (priznak && priznak1) 
                    {
                        generate_code(data, 18, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                        priznak1 = false;
                    }
                    else if (priznak)
                    {
                        generate_code(data, 18, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                    }
                    else if (priznak1)
                    {
                        generate_code(data, 18, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak1 = false;
                    }
                    else if (data->currentToken.secondType == ID_INT && data->code_generator_token.type == INT)
                    {
                        generate_code(data, 18, "", data->currentToken.attribute.i, 0, 0.1, "");
                    }
                    else if (data->currentToken.secondType == ID_DOUBLE && data->code_generator_token.type == DOUBLE) 
                    {
                        generate_code(data, 18, "", 1, 0, data->currentToken.attribute.d, "");
                    }
                    else if (data->currentToken.secondType == ID_STRING && data->code_generator_token.type == STRING)
                    {
                        generate_code(data, 18, "", 0, 0, 0.0, data->currentToken.attribute.s);
                    }
                }
                else if (!strcmp(data->operator, ">"))
                {
                    if (priznak && priznak1) 
                    {
                        generate_code(data, 19, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                        priznak1 = false;
                    }
                    else if (priznak)
                    {
                        generate_code(data, 19, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                    }
                    else if (priznak1)
                    {
                        generate_code(data, 19, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak1 = false;
                    }
                    else if (data->currentToken.secondType == ID_INT && data->code_generator_token.type == INT)
                    {
                        generate_code(data, 19, "", data->currentToken.attribute.i, 0, 0.2, "");
                    }
                    else if (data->currentToken.secondType == ID_DOUBLE && data->code_generator_token.type == DOUBLE)
                    {
                        generate_code(data, 19, "", 2, 0, data->currentToken.attribute.d, "");
                    }
                    else if (data->currentToken.secondType == ID_STRING && data->code_generator_token.type == STRING)
                    {
                        generate_code(data, 19, "", 0, 0, 0.0, data->currentToken.attribute.s);
                    }
                }
                else if (!strcmp(data->operator, ">="))
                {
                    if (priznak && priznak1) 
                    {
                        generate_code(data, 20, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                        priznak1 = false;
                    }
                    else if (priznak)
                    {
                        generate_code(data, 20, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                    }
                    else if (priznak1)
                    {
                        generate_code(data, 20, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak1 = false;
                    }
                    else if (data->currentToken.secondType == ID_INT && data->code_generator_token.type == INT)
                    {
                        generate_code(data, 20, "", data->currentToken.attribute.i, 0, 0.3, "");
                    }
                    else if (data->currentToken.secondType == ID_DOUBLE && data->code_generator_token.type == DOUBLE)
                    {
                        generate_code(data, 20, "", 3, 0, data->currentToken.attribute.d, "");
                    }
                    else if (data->currentToken.secondType == ID_STRING && data->code_generator_token.type == STRING)
                    {
                        generate_code(data, 20, "", 0, 0, 0.0, data->currentToken.attribute.s);
                    }
                }
                else if (!strcmp(data->operator, "=="))
                {
                    if (priznak && priznak1) 
                    {
                        generate_code(data, 21, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                        priznak1 = false;
                    }
                    else if (priznak)
                    {
                        generate_code(data, 21, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                    }
                    else if (priznak1)
                    {
                        generate_code(data, 21, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak1 = false;
                    }
                    else if (data->currentToken.secondType == ID_INT && data->code_generator_token.type == INT)
                    {
                        generate_code(data, 21, "", data->currentToken.attribute.i, 0, 0.3, "");
                    }
                    else if (data->currentToken.secondType == ID_DOUBLE && data->code_generator_token.type == DOUBLE)
                    {
                        generate_code(data, 21, "", 3, 0, data->currentToken.attribute.d, "");
                    }
                    else if (data->currentToken.secondType == ID_STRING && data->code_generator_token.type == STRING)
                    {
                        generate_code(data, 21, "", 0, 0, 0.0, data->currentToken.attribute.s);
                    }
                }
                else if (!strcmp(data->operator, "!="))
                {
                    if (priznak && priznak1) 
                    {
                        generate_code(data, 22, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                        priznak1 = false;
                    }
                    else if (priznak)
                    {
                        generate_code(data, 22, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak = false;
                    }
                    else if (priznak1)
                    {
                        generate_code(data, 22, "", GF_LF_choosing, GF_LF_choosing1, 0.0, "");
                        priznak1 = false;
                    }
                    else if (data->currentToken.secondType == ID_INT && data->code_generator_token.type == INT)
                    {
                        generate_code(data, 22, "", data->currentToken.attribute.i, 0, 0.3, "");
                    }
                    else if (data->currentToken.secondType == ID_DOUBLE && data->code_generator_token.type == DOUBLE)
                    {
                        generate_code(data, 22, "", 3, 0, data->currentToken.attribute.d, "");
                    }
                    else if (data->currentToken.secondType == ID_STRING && data->code_generator_token.type == STRING)
                    {
                        generate_code(data, 22, "", 0, 0, 0.0, data->currentToken.attribute.s);
                    }

                }
                data->flag = false;
            }

            if (data->currentToken.secondType == LESS)
            {
                data->flag = true;
                strcpy(data->operator, "<");
            }
            else if (data->currentToken.secondType == LESS_EQUAL)
            {
                data->flag = true;
                strcpy(data->operator, "<=");
            }
            else if (data->currentToken.secondType == MORE)
            {
                data->flag = true;
                strcpy(data->operator, ">");
            }
            else if (data->currentToken.secondType == MORE_EQUAL)
            {
                data->flag = true;
                strcpy(data->operator, ">=");
            }
            else if (data->currentToken.secondType == EQUAL)
            {
                data->flag = true;
                strcpy(data->operator, "==");
            }
            else if (data->currentToken.secondType == NOT_EQUAL)
            {
                data->flag = true;
                strcpy(data->operator, "!=");
            }



            Token shift;
            shift.type = LF_TYPE;
            shift.attribute.s = "<";
            tStack_Insert(data->stack, shift, Xth_from_top);

            tStack_Push(data->stack, data->currentToken);
            GET_TOKEN_EXPR(data->currentToken, data->lineCount);
            if (data->currentToken.type == LEFT_CURLY_B)
            {
                data->currentToken.type = DOLLAR;
            }
        }
        else if (precedenceTable[top.type][data->currentToken.type] == RG)
        {
            Token tmpToken[4];
            precedenceTableRulesStruct newRule = token2Rules(top);
            if (newRule.rule == NO_RULE)
            {
                return 2; //! neexistuje pro to pravidlo -> spatny vyraz
            }

            int i = 0;
            do
            {
                tStack_Pop(data->stack, &tmpToken[i++]);
            } while (tmpToken[i - 1].type != LF_TYPE && i != 4);
            

            int tmpType = NO_SECOND_TYPE;
            if (tmpToken[i - 1].type == LF_TYPE && i == newRule.size)
            {
                if (i == 2) // OPERAND
                {
                    if (tmpToken[0].type != IDENTIFIER)
                    {
                        return 2; //! <E
                    }
                    tmpType = tmpToken[0].secondType;
                }
                else if (i == 3) // NOT_NIL
                {
                    if (tmpToken[0].type != NOT_NIL || tmpToken[1].type != E_TOKEN)
                    {
                        return 2; //! <E!
                    }
                    tmpType = tmpToken[1].secondType;
                }
                else // if (i == 4)
                {
                    if ((tmpToken[0].type != E_TOKEN || tmpToken[2].type != E_TOKEN) && (tmpToken[0].type != RIGHT_BRACKET || tmpToken[1].type != E_TOKEN || tmpToken[2].type != LEFT_BRACKET))
                    {
                        return 2; //! <ExE || <(E)
                    }

                    if (tmpToken[0].type == E_TOKEN && tmpToken[2].type == E_TOKEN)
                    {
                        // <ExE
                        switch (tmpToken[1].type)
                        {
                            case NIL_CONDITION:
                                if (tmpToken[0].secondType == tmpToken[2].secondType)
                                {
                                    tmpType = tmpToken[0].secondType;
                                }
                                else
                                {
                                    return 7; //! nejsou stejneho typu
                                }
                                break;

                            case RELATION_COMPARE:
                                if (tmpToken[1].secondType == EQUAL || tmpToken[1].secondType == NOT_EQUAL)
                                {
                                    if (tmpToken[0].secondType == tmpToken[2].secondType)
                                    {
                                        tmpType = tmpToken[0].secondType;
                                    }
                                    else if ((tmpToken[0].secondType == ID_INT && tmpToken[2].secondType == ID_DOUBLE) || (tmpToken[0].secondType == ID_DOUBLE && tmpToken[2].secondType == ID_INT))
                                    {
                                        tmpType = ID_DOUBLE;
                                    }
                                    else
                                    {
                                        return 7; //! nejsou preveditelneho typu
                                    }
                                }
                                else
                                {
                                    if (tmpToken[0].secondType == tmpToken[2].secondType)
                                    {
                                        tmpType = tmpToken[0].secondType;
                                    }
                                    else
                                    {
                                        return 7; //! nejsou stejneho typu
                                    }
                                }
                                break;

                            case PLUS_MINUS:
                                if (tmpToken[0].secondType == tmpToken[2].secondType)
                                {
                                    if (tmpToken[1].secondType == MINUS && tmpToken[0].secondType == ID_STRING)
                                    {
                                        return 7; //! konkatenace je pouze u PLUS a ne MINUS
                                    }
                                    tmpType = tmpToken[0].secondType;
                                }
                                else if ((tmpToken[0].secondType == ID_INT && tmpToken[2].secondType == ID_DOUBLE) || (tmpToken[0].secondType == ID_DOUBLE && tmpToken[2].secondType == ID_INT))
                                {
                                    tmpType = ID_DOUBLE;
                                }
                                else
                                {
                                    return 7; //! nejsou preveditelneho typu
                                }
                                break;

                            case MULTIPLY_DIVIDE:
                                if (tmpToken[0].secondType == tmpToken[2].secondType)
                                {
                                    tmpType = tmpToken[0].secondType;
                                }
                                else if ((tmpToken[0].secondType == ID_INT && tmpToken[2].secondType == ID_DOUBLE) || (tmpToken[0].secondType == ID_DOUBLE && tmpToken[2].secondType == ID_INT))
                                {
                                    tmpType = ID_DOUBLE;
                                }
                                else
                                {
                                    return 7; //! nejsou preveditelneho typu
                                }
                                break;

                            default:
                                break;
                        }
                    }
                    else
                    {
                        // <(E)
                        tmpType = tmpToken[1].secondType;
                    }
                }
                tmpToken[i - 1].type = E_TOKEN; // teoreticky muzu pouzit kterykoliv predchozi, protoze uz je nepotrebuju
                tmpToken[i - 1].attribute.s = "E";
                tmpToken[i - 1].secondType = tmpType;
                tStack_Push(data->stack, tmpToken[i - 1]);
                // fprintf(stderr, "Rule number: %d\n", newRule.rule); //! Printing rule number  //! DEBUG
            }
            else
            {
                return 2; //! spatny vyraz
            }





        }
        else if (precedenceTable[top.type][data->currentToken.type] == EQU)
        {
            tStack_Push(data->stack, data->currentToken);
            GET_TOKEN_EXPR(data->currentToken, data->lineCount);
            if (data->currentToken.type == LEFT_CURLY_B)
            {
                data->currentToken.type = DOLLAR;
            }
        }
        else // if (precedenceTable[top.type][data->currentToken.type] == UN)
        {
            return 2; //! nemame pravidlo -> spatny vyraz
        }
    } while (!(data->currentToken.type == DOLLAR && data->stack->tokens[0].type == DOLLAR && data->stack->tokens[1].type == E_TOKEN && data->stack->size == 2));
    data->currentToken.type = LEFT_CURLY_B;
    tStack_Dispose(data->stack);
    if (!relation)
    {
        return 7; //! nelze u podminky urcit pravdivostni hodnotu
    }
    return 0;
}

/** Hlavni funkce pro zpracovavani vyrazu do EOL, COMMA... Vraci error_number nebo 0 */
int valueExpression(parserData *data, tokenType expectedTokenType, int *selectType)
{
    Token dollar;
    dollar.type = DOLLAR;
    dollar.attribute.s = "$";

    Token secondToken;
    secondToken.type = NO_TYPE;
    if (data->stack->size == 1)
    {
        secondToken = data->currentToken;
        tStack_Pop(data->stack, &(data->currentToken));
    }
    tStack_Dispose(data->stack);
    tStack_Init(data->stack);
    tStack_Push(data->stack, dollar);

    bool first = true;
    tokenType expectedTokenSecondType = NO_TYPE;
    if(expectedTokenType == COMMA)
    {
        expectedTokenSecondType = RIGHT_BRACKET;
    }
    else if (expectedTokenType == EOL_TYPE)
    {
        expectedTokenSecondType = RIGHT_CURLY_B;
    }
    else if (expectedTokenType == E_TOKEN)
    {
        Token tokenE;
        tokenE.type = E_TOKEN;
        if (!strcmp(data->value_type, "Int"))
        {
            tokenE.secondType = ID_INT;
        }
        else if (!strcmp(data->value_type, "Int?"))
        {
            tokenE.secondType = ID_INT;
        }
        else if (!strcmp(data->value_type, "Double"))
        {
            tokenE.secondType = ID_DOUBLE;
        }
        else if (!strcmp(data->value_type, "Double?"))
        {
            tokenE.secondType = ID_DOUBLE;
        }
        else if (!strcmp(data->value_type, "String"))
        {
            tokenE.secondType = ID_STRING;
        }
        else if (!strcmp(data->value_type, "String?"))
        {
            tokenE.secondType = ID_STRING;
        }
        else
        {
            return 8; //! promenna nema typ
        }
        tokenE.attribute.s = "E";
        tStack_Push(data->stack, tokenE);
        expectedTokenType = EOL_TYPE;
        expectedTokenSecondType = RIGHT_CURLY_B;
    }

    Token top;
    int Xth_from_top = 0;
    do
    {
        if (data->currentToken.type == EOF_TYPE)
        {
            data->currentToken.type = DOLLAR;
        }

        tStack_Top(data->stack, &top);
        Xth_from_top = 0;
        if (top.type == E_TOKEN)
        {
            tStack_Second_Top(data->stack, &top);
            Xth_from_top = 1;
        }

        bool priznak = false;
        if (data->currentToken.type == INT)
        {
            data->currentToken.secondType = ID_INT;
            data->currentToken.type = IDENTIFIER;
        }
        else if (data->currentToken.type == DOUBLE || data->currentToken.type == DOUBLE_EXPONENT)
        {
            data->currentToken.secondType = ID_DOUBLE;
            data->currentToken.type = IDENTIFIER;
        }
        else if (data->currentToken.type == STRING || data->currentToken.type == MORE_LINES_STRING)
        {
            data->currentToken.secondType = ID_STRING;
            data->currentToken.type = IDENTIFIER;
        }
        else if (data->currentToken.type == IDENTIFIER)
        {
            priznak = true;
            int typeID = findTypeOfID(data);
            if (typeID >= 80)
            {
                return typeID - 80;
            }
            data->currentToken.secondType = typeID;
        }

        if (precedenceTable[top.type][data->currentToken.type] == LF)
        {
            int GF_LF_choosing = 0; 
            if (priznak) 
            {
                Node *searching = searchNode(data->symTable[0], data->currentToken.attribute.s);
                if (searching == NULL) 
                {
                    GF_LF_choosing = 1;
                }
                else
                {
                    GF_LF_choosing = 0;
                }
            }
            if (data->flag) 
            {
                if (!strcmp(data->operator, "+"))
                {
                    if (priznak) {
                        generate_code(data, 15, data->idOfVar, GF_LF_choosing, 0, 0.0, data->currentToken.attribute.s);
                        priznak = false;
                    }
                    else if (data->currentToken.secondType == ID_INT)
                    {
                        generate_code(data, 7, data->idOfVar, data->currentToken.attribute.i, 0, 0.0, "");
                    }
                    else if (data->currentToken.secondType == ID_DOUBLE)
                    {
                        generate_code(data, 13, data->idOfVar, 0, 0, data->currentToken.attribute.d, "");
                    }
                    else if (data->currentToken.secondType == ID_STRING)
                    {
                        generate_code(data, 14, data->idOfVar, 0, 0, 0.0, data->currentToken.attribute.s);
                    }
                }
                else if (!strcmp(data->operator, "-"))
                {
                    if (data->currentToken.secondType == ID_INT)
                    {
                        generate_code(data, 7, data->idOfVar, data->currentToken.attribute.i, 0, 0.1, "");
                    }
                    else if (data->currentToken.secondType == ID_DOUBLE)
                    {
                        generate_code(data, 13, data->idOfVar, 1, 0, data->currentToken.attribute.d, "");
                    } 
                }
                else if (!strcmp(data->operator, "*"))
                {
                    if (data->currentToken.secondType == ID_INT)
                    {
                        generate_code(data, 7, data->idOfVar, data->currentToken.attribute.i, 0, 0.2, "");
                    }
                    else if (data->currentToken.secondType == ID_DOUBLE)
                    {
                        generate_code(data, 13, data->idOfVar, 2, 0, data->currentToken.attribute.d, "");
                    }
                }
                else if (!strcmp(data->operator, "/"))
                {
                    if (data->currentToken.secondType == ID_INT)
                    {
                        generate_code(data, 7, data->idOfVar, data->currentToken.attribute.i, 0, 0.3, "");
                    }
                    else if (data->currentToken.secondType == ID_DOUBLE)
                    {
                        generate_code(data, 13, data->idOfVar, 3, 0, data->currentToken.attribute.d, "");
                    }
                }
                data->flag = false;
            }

            if (data->currentToken.secondType == PLUS)
            {
                data->flag = true;
                strcpy(data->operator, "+");
            }
            else if (data->currentToken.secondType == MINUS)
            {
                data->flag = true;
                strcpy(data->operator, "-");
            }
            else if (data->currentToken.secondType == MULTIPLY)
            {
                data->flag = true;
                strcpy(data->operator, "*");
            }
            else if (data->currentToken.secondType == DIVIDE)
            {
                data->flag = true;
                strcpy(data->operator, "/");
            }

            Token shift;
            shift.type = LF_TYPE;
            shift.attribute.s = "<";
            tStack_Insert(data->stack, shift, Xth_from_top);

            tStack_Push(data->stack, data->currentToken);
            if (secondToken.type == NO_TYPE)
            {
                GET_TOKEN_EXPR(data->currentToken, data->lineCount);
            }
            else
            {
                data->currentToken = secondToken;
                secondToken.type = NO_TYPE;
            }

            if (data->currentToken.type == expectedTokenType)
            {
                first = true;
                data->currentToken.type = DOLLAR;
            }
            else if (data->currentToken.type == expectedTokenSecondType)
            {
                first = false;
                data->currentToken.type = DOLLAR;
            }
        }
        else if (precedenceTable[top.type][data->currentToken.type] == RG)
        {
            Token tmpToken[4];
            precedenceTableRulesStruct newRule = token2Rules(top);
            if (newRule.rule == NO_RULE)
            {
                return 2; //! neexistuje pro to pravidlo -> spatny vyraz
            }

            int i = 0;
            do
            {
                tStack_Pop(data->stack, &tmpToken[i++]);
            } while (tmpToken[i - 1].type != LF_TYPE && i != 4);

            int tmpType = NO_SECOND_TYPE;
            if (tmpToken[i - 1].type == LF_TYPE && i == newRule.size)
            {
                if (i == 2) // OPERAND
                {
                    if (tmpToken[0].type != IDENTIFIER)
                    {
                        return 2; //! <E
                    }
                    tmpType = tmpToken[0].secondType;
                }
                else if (i == 3) // NOT_NIL
                {
                    if (tmpToken[0].type != NOT_NIL || tmpToken[1].type != E_TOKEN)
                    {
                        return 2; //! <E!
                    }
                    tmpType = tmpToken[1].secondType;
                }
                else // if (i == 4)
                {
                    if ((tmpToken[0].type != E_TOKEN || tmpToken[2].type != E_TOKEN) && (tmpToken[0].type != RIGHT_BRACKET || tmpToken[1].type != E_TOKEN || tmpToken[2].type != LEFT_BRACKET))
                    {
                        return 2; //! <ExE || <(E)
                    }

                    if (tmpToken[0].type == E_TOKEN && tmpToken[2].type == E_TOKEN)
                    {
                        // <ExE
                        switch (tmpToken[1].type)
                        {
                            case NIL_CONDITION:
                                if (tmpToken[0].secondType == tmpToken[2].secondType)
                                {
                                    tmpType = tmpToken[0].secondType;
                                }
                                else
                                {
                                    return 7; //! nejsou stejneho typu
                                }
                                break;

                            case RELATION_COMPARE:
                                if (tmpToken[1].secondType == EQUAL || tmpToken[1].secondType == NOT_EQUAL)
                                {
                                    if (tmpToken[0].secondType == tmpToken[2].secondType)
                                    {
                                        tmpType = tmpToken[0].secondType;
                                    }
                                    else if ((tmpToken[0].secondType == ID_INT && tmpToken[2].secondType == ID_DOUBLE) || (tmpToken[0].secondType == ID_DOUBLE && tmpToken[2].secondType == ID_INT))
                                    {
                                        tmpType = ID_DOUBLE;
                                    }
                                    else
                                    {
                                        return 7; //! nejsou preveditelneho typu
                                    }
                                }
                                else
                                {
                                    if (tmpToken[0].secondType == tmpToken[2].secondType)
                                    {
                                        tmpType = tmpToken[0].secondType;
                                    }
                                    else
                                    {
                                        return 7; //! nejsou stejneho typu
                                    }
                                }
                                break;

                            case PLUS_MINUS:
                                if (tmpToken[0].secondType == tmpToken[2].secondType)
                                {
                                    if (tmpToken[1].secondType == MINUS && tmpToken[0].secondType == ID_STRING)
                                    {
                                        return 7; //! konkatenace je pouze u PLUS a ne MINUS
                                    }
                                    tmpType = tmpToken[0].secondType;
                                }
                                else if ((tmpToken[0].secondType == ID_INT && tmpToken[2].secondType == ID_DOUBLE) || (tmpToken[0].secondType == ID_DOUBLE && tmpToken[2].secondType == ID_INT))
                                {
                                    tmpType = ID_DOUBLE;
                                }
                                else
                                {
                                    return 7; //! nejsou preveditelneho typu
                                }
                                break;

                            case MULTIPLY_DIVIDE:
                                if (tmpToken[0].secondType == tmpToken[2].secondType)
                                {
                                    tmpType = tmpToken[0].secondType;
                                }
                                else if ((tmpToken[0].secondType == ID_INT && tmpToken[2].secondType == ID_DOUBLE) || (tmpToken[0].secondType == ID_DOUBLE && tmpToken[2].secondType == ID_INT))
                                {
                                    tmpType = ID_DOUBLE;
                                }
                                else
                                {
                                    return 7; //! nejsou preveditelneho typu
                                }
                                break;

                            default:
                                break;
                        }
                    }
                    else
                    {
                        // <(E)
                        tmpType = tmpToken[1].secondType;
                    }
                }
                tmpToken[i - 1].type = E_TOKEN; // teoreticky muzu pouzit kterykoliv predchozi, protoze uz je nepotrebuju
                tmpToken[i - 1].attribute.s = "E";
                tmpToken[i - 1].secondType = tmpType;
                tStack_Push(data->stack, tmpToken[i - 1]);
                // fprintf(stderr, "Rule number: %d\n", newRule.rule); //! Printing rule number  //! DEBUG
            }
            else
            {
                return 2; //! spatny vyraz
            }
        }
        else if (precedenceTable[top.type][data->currentToken.type] == EQU)
        {
            tStack_Push(data->stack, data->currentToken);
            if (secondToken.type == NO_TYPE)
            {
                GET_TOKEN_EXPR(data->currentToken, data->lineCount);
            }
            else
            {
                data->currentToken = secondToken;
                secondToken.type = NO_TYPE;
            }
            
            if (data->currentToken.type == expectedTokenType)
            {
                first = true;
                data->currentToken.type = DOLLAR;
            }
            else if (data->currentToken.type == expectedTokenSecondType)
            {
                first = false;
                data->currentToken.type = DOLLAR;
            }
        }
        else // if (precedenceTable[top.type][data->currentToken.type] == UN)
        {
            return 2; //! nemame pravidlo -> spatny vyraz
        }
    } while (!(data->currentToken.type == DOLLAR && data->stack->tokens[0].type == DOLLAR && data->stack->tokens[1].type == E_TOKEN && data->stack->size == 2));
    if (first)
    {
        data->currentToken.type = expectedTokenType;
    }
    else
    {
        data->currentToken.type = expectedTokenSecondType;
    }
    *selectType = data->stack->tokens[1].secondType * 2 - 21;
    tStack_Dispose(data->stack);
    // fprintf(stderr, "\n"); //! DEBUG
    return 0;
}
