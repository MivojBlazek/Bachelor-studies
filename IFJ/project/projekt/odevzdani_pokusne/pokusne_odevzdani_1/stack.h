/**
 * @file stack.h
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
 * @author <xmicha94> Kryštof Michálek
 * @author <xsapik02> Matyáš Sapík
 * @author <xzavod16> Ondřej Závodský
*/

#ifndef _IFJ_STACK_H_
#define _IFJ_STACK_H_

#include "scanner.h"
#include <stdbool.h>
#include "symtable.h"

/** Velikost zasobniku pro prvni alokaci a pripadne rozsireni */
#define STACK_SIZE 200

/** Promenna udavajici error */
extern bool stack_error;

/** Zasobnik polozek */
typedef struct
{
    Node *tree;
    unsigned size;
}SymtabStack;

/** Zasobnik tokenu */
typedef struct
{
    Token *tokens;
    unsigned size;
}tokenStack;



/**
 * Zasobnikove operace s polozkami
*/

/** Inicializace zasobniku */
void Stack_Init(SymtabStack *stack);

/** Precte polozku na vrcholu zasobniku */
void Stack_Top(SymtabStack *stack, Node *tree);

/** Vezme polozku z vrcholu zasobniku, vymaze ji ze zasobniku a vrati ji */
void Stack_Pop(SymtabStack *stack, Node *tree);

/** Vlozi polozku na vrchol zasobniku */
void Stack_Push(SymtabStack *stack, Node tree);

/** Vycisteni zasobniku */
void Stack_Dispose(SymtabStack *stack);



/**
 * Zasobnikove operace nad tokeny
*/

/** Inicializace zasobniku */
void tStack_Init(tokenStack *stack);

/** Precte token na vrcholu zasobniku */
void tStack_Top(tokenStack *stack, Token *token);

/** Precte token na 2. miste z vrcholu zasobniku */
void tStack_Second_Top(tokenStack *stack, Token *token);

/** Vezme token z vrcholu zasobniku, vymaze ji ze zasobniku a vrati ji */
void tStack_Pop(tokenStack *stack, Token *token);

/** Vlozi token na vrchol zasobniku */
void tStack_Push(tokenStack *stack, Token token);

/** Vlozi token na misto x-te misto od shora */
void tStack_Insert(tokenStack *stack, Token token, unsigned int x);

/** Vycisteni zasobniku */
void tStack_Dispose(tokenStack *stack);

#endif //_IFJ_STACK_H_