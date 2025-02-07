/**
 * @file stack.c
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
*/

#include "stack.h"
#include <stdlib.h>

/** Promenna pro zachytavani erroru */
bool stack_error = false;



/**
 * Zasobnikove operace s polozkami
*/

/** Inicializace zasobniku */
void Stack_Init(SymtabStack *stack)
{
    if (!stack)
    {
        stack_error = true;
        return;
    }
    stack->size = 0;
    stack->tree = NULL;
}

/** Precte polozku na vrcholu zasobniku */
void Stack_Top(SymtabStack *stack, Node *tree)
{
    if (stack->tree && stack->size > 0)
    {
        *tree = stack->tree[stack->size - 1];
        return;
    }
    stack_error = true;
    tree = NULL;
}

/** Vezme polozku z vrcholu zasobniku, vymaze ji ze zasobniku a vrati ji */
void Stack_Pop(SymtabStack *stack, Node *tree)
{
    Stack_Top(stack, tree);
    stack->size--;
}

/** Vlozi polozku na vrchol zasobniku, pripadne si ho rozsiri */
void Stack_Push(SymtabStack *stack, Node tree)
{
    if (stack->size % STACK_SIZE == STACK_SIZE - 1)
    {
        stack->tree = (Node *)realloc(stack->tree, sizeof(Node) * (stack->size + 1 + STACK_SIZE));
        if (!stack->tree)
        {
            free(stack->tree);
            stack_error = true;
            return;
        }
    }
    stack->tree[stack->size++] = tree;
}

/** Vycisteni zasobniku */
void Stack_Dispose(SymtabStack *stack)
{
    if (stack->tree)
    {
        free(stack->tree);
        stack->tree = NULL;
        stack->size = 0;
    }
}



/**
 * Zasobnikove operace s tokeny
*/

/** Inicializace zasobniku */
void tStack_Init(tokenStack *stack)
{
    if (!stack)
    {
        stack_error = true;
        return;
    }
    stack->size = 0;
    stack->tokens = (Token *)malloc(sizeof(Token) * STACK_SIZE);
    if (!stack->tokens)
    {
        stack_error = true;
    }
}

/** Precte token na vrcholu zasobniku */
void tStack_Top(tokenStack *stack, Token *token)
{
    if (stack->tokens && stack->size > 0)
    {
        *token = stack->tokens[stack->size - 1];
        return;
    }
    stack_error = true;
    token = NULL;
}

/** Precte token na 2. msite na vrcholu zasobniku */
void tStack_Second_Top(tokenStack *stack, Token *token)
{
    if (stack->tokens && stack->size > 1)
    {
        *token = stack->tokens[stack->size - 2];
        return;
    }
    stack_error = true;
    token = NULL;
}

/** Vezme token z vrcholu zasobniku, vymaze ji ze zasobniku a vrati ji */
void tStack_Pop(tokenStack *stack, Token *token)
{
    tStack_Top(stack, token);
    stack->size--;
}

/** Vlozi token na vrchol zasobniku, pripadne si ho rozsiri */
void tStack_Push(tokenStack *stack, Token token)
{
    if (stack->size % STACK_SIZE == STACK_SIZE - 1)
    {
        stack->tokens = (Token *)realloc(stack->tokens, sizeof(Token) * (stack->size + 1 + STACK_SIZE));
        if (!stack->tokens)
        {
            free(stack->tokens);
            stack_error = true;
            return;
        }
    }
    stack->tokens[stack->size++] = token;
}

/** Vlozi token na x-te misto od shora */
void tStack_Insert(tokenStack *stack, Token token, unsigned int x)
{
    Token tmp[x];
    for (unsigned int i = 0; i < x; i++)
    {
        tStack_Pop(stack, &tmp[i]);
    }
    tStack_Push(stack, token);
    for (int i = x - 1; i >= 0; i--)
    {
        tStack_Push(stack, tmp[i]);
    }
}

/** Vycisteni zasobniku */
void tStack_Dispose(tokenStack *stack)
{
    if (stack->tokens)
    {
        free(stack->tokens);
        stack->tokens = NULL;
        stack->size = 0;
    }
}