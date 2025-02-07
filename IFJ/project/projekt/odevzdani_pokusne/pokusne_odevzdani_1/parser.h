/**
 * @file parser.h
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
 * @author <xmicha94> Kryštof Michálek
 * @author <xsapik02> Matyáš Sapík
 * @author <xzavod16> Ondřej Závodský
*/

#ifndef _PARSER_H_
#define _PARSER_H_

#include "scanner.h"
#include "stack.h"
#include "symtable.h"

typedef struct
{
    Token currentToken;     // aktualne zpracovavany token
    tokenStack *stack;      // zasobnik tokenu, vyuzivany pri predavani tokenu expressions
    Node *symTable[255];    // pole tabulek symbolu
    int tsCount;            // pocet aktivnich tabulek symbolu
    int paramCount;         // pocet parametru
    bool returning;         // true pokud ocekavame, ze funkce bude vracet hodntu (tj. ocekavame return ...)
    char *expected_return;  // ocekavany return typ ve funkci
    int lineCount;          // cislo radku, ktere ziskavame of scanneru
    char *value_type;       // vraci typ z nekterych funkci (napr. assignment() - zjistime typ z prirazeni a porovname s aktualnim, ...)
} parserData;

/** Funkce, ktera prida vestavene funkce do globalni tabulky symbolu */
void addInternalFunc(Node **tree);

/** Blok funkci reprezentujicich LL-gramatiku */

    /** <PROG> */
    int prog(parserData *data);

    /** <BLOCK> */
    int block(parserData *data);

    /** <VOID_OR_ASSIGN> */
    int void_or_assign(parserData *data, Token *funcName);

    /** <ELSE> */
    int elsing(parserData *data);

    /** <EOL> */
    int eol(parserData *data);

    /** <PARAM> */
    int param(parserData *data, Node *nodeToChange);

    /** <NEXT_PARAM> */
    int next_param(parserData *data, Node *nodeToChange);

    /** <RETURN> */
    int returning(parserData *data, Node *nodeToChange);

    /** <ST_LIST> */
    int st_list(parserData *data);

    /** <STATEMENT> */
    int statement(parserData *data);

    /** <DATA_TYPE> */
    int data_type(parserData *data, Param *paramToSave);

    /** <ASSIGN_MARK> */
    int assign_mark(parserData *data);

    /** <ASSIGNMENT> */
    int assignment(parserData *data);

    /** <CALLING_PARAM> */
    int calling_param(parserData *data, Token *funcName);

    /** <WITH_VALUE> */
    int with_value(parserData *data);

    /** <NEXT_CALLING> */
    int next_calling(parserData *data, Token *funcName);

    /** <VALUE> */
    int value(parserData *data, tokenType expectedType);

    /** <WITH_TYPE> */
    int with_type(parserData *data, Node *nodeToChange);

/** Konec bloku */

/** Hlavni funkce celeho prekladace */
int startCompiling();

#endif //_PARSER_H_