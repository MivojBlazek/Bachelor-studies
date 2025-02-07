/**
 * @file symtable.h
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
*/

#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <stdbool.h>

/** Parametr funkce */
typedef struct
{
	char *callingName;
	char *name;
	char *type;
} Param;

/** Uzel stromu tabulky symbolu */
typedef struct bst_node
{
  char *name;               // jmeno (klic 1)
  char *type;               // datovy typ
  bool func;				        // funkce - true, promenna - false
  bool const_let;           // let - true, jine - false
  Param *params;		        // parametry funkci //! mby udelat jako seznam
  struct bst_node *left;    // levy potomek
  struct bst_node *right;   // pravy potomek
} Node;

/** Promenna urcujici prepsani v tabulce symbolu pri insertNode() */
extern bool changing;


/** Inicializace stromu */
void initTree(Node **tree);

/** Funkce najde uzel podle jmena, jinak vraci NULL */
Node *searchNode(Node *tree, char *name);

/** Funkce vlozi do stromu novy uzel a vyskove vyvazi strom */
void insertNode(Node **tree, Node *insert);

/** Funkce vycisti cely strom */
void disposeTree(Node **tree);

/** Funkce vyvazi strom */
void balanceTree(Node **tree);

#endif // _SYMTABLE_H_
