/**
 * @file expression.c
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
 * @author <xmicha94> Kryštof Michálek
 * @author <xsapik02> Matyáš Sapík
 * @author <xzavod16> Ondřej Závodský
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

    do
    {
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

        if (data->currentToken.type == INT || data->currentToken.type == DOUBLE || data->currentToken.type == DOUBLE_EXPONENT || data->currentToken.type == STRING || data->currentToken.type == MORE_LINES_STRING)
        {
            data->currentToken.secondType = data->currentToken.type - INT + ID_INT;
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

        if (precedenceTable[top.type][data->currentToken.type] == LF)
        {
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
                return 6; //! neexistuje pro to pravidlo -> spatny vyraz
            }

            int i = 0;
            do
            {
                tStack_Pop(data->stack, &tmpToken[i++]);
            } while (tmpToken[i - 1].type != LF_TYPE && i != 4);
            
            if (tmpToken[i - 1].type == LF_TYPE && i == newRule.size)
            {
                if (i == 2) // OPERAND
                {
                    if (tmpToken[0].type != IDENTIFIER)
                    {
                        return 6; //! <E
                    }
                }
                else if (i == 3) // NOT_NIL
                {
                    if (tmpToken[0].type != NOT_NIL || tmpToken[1].type != E_TOKEN)
                    {
                        return 6; //! <E!
                    }
                }
                else // if (i == 4)
                {
                    if ((tmpToken[0].type != E_TOKEN || tmpToken[2].type != E_TOKEN) && (tmpToken[0].type != RIGHT_BRACKET || tmpToken[1].type != E_TOKEN || tmpToken[2].type != LEFT_BRACKET))
                    {
                        return 6; //! <ExE || <(E)
                    }
                }
                tmpToken[i - 1].type = E_TOKEN; // teoreticky muzu pouzit kterykoliv predchozi, protoze uz je nepotrebuju
                tmpToken[i - 1].attribute.s = "E";
                tStack_Push(data->stack, tmpToken[i - 1]);
                // fprintf(stderr, "Rule number: %d\n", newRule.rule); //! Printing rule number  //! DEBUG
            }
            else
            {
                return 6; //! spatny vyraz
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
        return 2; //! nelze u podminky urcit pravdivostni hodnotu
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
            return 7; //! promenna nema typ
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
        tStack_Top(data->stack, &top);
        Xth_from_top = 0;
        if (top.type == E_TOKEN)
        {
            tStack_Second_Top(data->stack, &top);
            Xth_from_top = 1;
        }

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
            int typeID = findTypeOfID(data);
            if (typeID >= 80)
            {
                return typeID - 80;
            }
            data->currentToken.secondType = typeID;
        }

        if (precedenceTable[top.type][data->currentToken.type] == LF)
        {
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
                return 6; //! neexistuje pro to pravidlo -> spatny vyraz
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
                        return 6; //! <E
                    }
                    tmpType = tmpToken[0].secondType;
                }
                else if (i == 3) // NOT_NIL
                {
                    if (tmpToken[0].type != NOT_NIL || tmpToken[1].type != E_TOKEN)
                    {
                        return 6; //! <E!
                    }
                    tmpType = tmpToken[1].secondType;
                }
                else // if (i == 4)
                {
                    if ((tmpToken[0].type != E_TOKEN || tmpToken[2].type != E_TOKEN) && (tmpToken[0].type != RIGHT_BRACKET || tmpToken[1].type != E_TOKEN || tmpToken[2].type != LEFT_BRACKET))
                    {
                        return 6; //! <ExE || <(E)
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
                                if (tmpToken[0].secondType == tmpToken[2].secondType)
                                {
                                    tmpType = tmpToken[0].secondType;
                                }
                                else
                                {
                                    return 7; //! nejsou stejneho typu
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
                return 6; //! spatny vyraz
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
