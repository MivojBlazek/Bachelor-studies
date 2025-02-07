/**
 * @file expression.h
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
 * @author <xmicha94> Kryštof Michálek
*/

#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "scanner.h"
#include "parser.h"

/** Znacky v precedencni tabulce */
typedef enum
{
    EQU, // = equal
    LF,  // < left shift
    RG,  // > right reduce
    UN   // X undefinied
}precedenceTableSign;

/** Precedencni pravidla */
typedef enum Rules
{
    E_PLUS_E = 0,       // 0:  E --> E + E
    E_MINUS_E,          // 1:  E --> E - E
    E_MULTIPLY_E,       // 2:  E --> E * E
    E_DIVIDE_E,         // 3:  E --> E / E
    LEFTPAR_E_RIGHTPAR, // 4:  E --> (E)
    OPERAND,            // 5:  E --> i
    E_NIL_CONDITION_E,  // 6:  E --> E ?? E
    E_NOT_NIL,          // 7:  E --> E !
    E_EQUAL_E,          // 8:  E --> E == E
    E_NOT_EQUAL_E,      // 9:  E --> E != E
    E_LESS_E,           // 10: E --> E < E
    E_MORE_E,           // 11: E --> E > E
    E_LESS_EQUAL_E,     // 12: E --> E <= E
    E_MORE_EQUAL_E,     // 13: E --> E >= E
    E,                  // 14: E (only E left on stack)
    NO_RULE             // 15: No rule
}precedenceTableRules;

/** Struktura pravidla a poctu tokenu potrebnych na toto pravidlo */
typedef struct
{
    precedenceTableRules rule;
    int size;
}precedenceTableRulesStruct;

/** Funkce pro prevod typu tokenu na precedencni pravidla */
precedenceTableRulesStruct token2Rules(Token token);

/** Funkce najde datovy typ ID v TS a vrati ho */
int findTypeOfID(parserData *data);

/** Hlavni funkce pro zpracovavani vyrazu do {. Vraci error_number nebo 0 */
int expression(parserData *data);

/** Hlavni funkce pro zpracovavani vyrazu do EOL, COMMA... Vraci error_number nebo 0 a pomoci selectType vraci datovy typ */
int valueExpression(parserData *data, tokenType expectedTokenType, int *selectType);

#endif //_EXPRESSION_H_