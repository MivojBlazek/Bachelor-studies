/**
 * @file symtable.c
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
 * @author ...
 * @author ...
 * @author ...
*/

#include "symtable.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

errorT = 0;

/** Inicializace stromu */
void initTree(Node **tree)
{
    (*tree) = NULL;
}

/** Funkce vlozi do stromu novy uzel */
void insertNode(Node **tree, char *name, char *block, char *type, char *value)
{
    if ((*tree) == NULL)
    {
        (*tree) = (Node *)malloc(sizeof(Node));
        if (*tree == NULL)
        {
            //! malloc failed
            errorT = 1;
            return;
        }

        (*tree)->left = NULL;
        (*tree)->right = NULL;
        (*tree)->name = name;
        (*tree)->block = block;
        (*tree)->type = type;
        (*tree)->value = value;
    }
    else
    {
        int result = strcmp((*tree)->name, name);
        if (result < 0) //TODO zkontrolovat podminku
        {
            insertNode(&(*tree)->left, name, block, type, value);
        }
        else
        {
            insertNode(&(*tree)->right, name, block, type, value);
        }
    }
}

/** Funkce najde nejpravejsi aktivni uzel podle klice, jinak vraci NULL */
Node *searchTheRightmostNode(Node *tree, char *name)
{
    Node *new = NULL;
    if (tree != NULL)
    {
        new = searchTheRightmostNode(tree->left, name);
        if (!strcmp(tree->name, name) && tree->active)
        {
            return tree;
        }
        new = searchTheRightmostNode(tree->right, name);
    }
    return new;
}

/** Funkce vycisti cely strom */
void disposeTree(Node **tree)
{
    if (*tree != NULL)
    {
        disposeTree(&((*tree)->left));
        disposeTree(&((*tree)->right));
        free(*tree);
        *tree = NULL;
    }
}

/** Funkce udela vsechny uzly z jednoho bloku neaktivni */
void makeBlockInactive(Node **tree, char *block)
{
    if (tree != NULL)
    {
        makeBlockInactive(&((*tree)->left), block);
        makeBlockInactive(&((*tree)->right), block);
        if (!strcmp((*tree)->block, block))
        {
            (*tree)->active = false;
        }
    }
}

/** Funkce vytiskne uzel */
void printNode(Node *node)
{
    printf("Node:\n   name=\"%s\"\n   block=\"%s\"\n   active=%d\n   type=\"%s\"\n   value=\"%s\"\n   left=\"%s\"\n   right=\"%s\"\n",
                      node->name,     node->block,     node->active, node->type,     node->value,     node->left ? node->left->name : "NULL", node->right ? node->right->name : "NULL");
}

/** Pomocna funkce, ktera provede absolutni hodnotu z cisla */
int abs(int number)
{
    return number < 0 ? -number : number;
}

/**
 * Pomocna funkce provede dostatecny pocet spravnych rotaci
 * 
 * @param tree je ukazatel na kriticky uzel
 * @param situation udava situaci ktera nastala
 * 
 * @returns upraveny strom
*/
void doBalance(Node **tree, int situation)
{
    Node *tmp;
    Node *tmpSon;
    switch (situation) // 1:LL, 2:LR, 3:RL, 4:RR
    {
        case 1: // LL rotace doprava
            tmp = (*tree)->left->right;
            tmpSon = (*tree)->left;
            (*tree)->left->right = *tree;
            (*tree)->left = tmp;
            break;

        case 2: // LR
            // 1. rotace (synu doleva)
            tmp = (*tree)->left->right->left;
            tmpSon = (*tree)->left->right;
            (*tree)->left->right->left = (*tree)->left;
            (*tree)->left->right = tmp;
            (*tree)->left = tmpSon;

            // 2. rotace (doprava)
            tmp = (*tree)->left->right;
            tmpSon = (*tree)->left;
            (*tree)->left->right = *tree;
            (*tree)->left = tmp;
            break;

        case 3: // RL
            // 1. rotace (synu doprava)
            tmp = (*tree)->right->left->right;
            tmpSon = (*tree)->right->left;
            (*tree)->right->left->right = (*tree)->right;
            (*tree)->right->left = tmp;
            (*tree)->right = tmpSon;

            // 2. rotace (doleva)
            tmp = (*tree)->right->left;
            tmpSon = (*tree)->right;
            (*tree)->right->left = *tree;
            (*tree)->right = tmp;
            break;

        case 4: // RR rotace doleva
            tmp = (*tree)->right->left;
            tmpSon = (*tree)->right;
            (*tree)->right->left = *tree;
            (*tree)->right = tmp;
            break;

        default:
            return;
    }
    *tree = tmpSon;
}

/**
 * Pomocna funkce ktera zjisti, jestli binarni strom je vyskove vyvazeny
 * 
 * @param tree ocekava strom pro ktery bude zjistovat vyvazenost
 * @param count vraci vysku stromu
 * @param criticalState urcuje ktery stav nastal (podle tohe pak vyvazime)
 * 
 * @returns true - pokud je vyvazeny \n
 * @returns false - pokud neni vyvazeny
*/
bool isHeightBalanced(Node *tree, int *count, int *criticalState)
{
    bool leftBalanced, rightBalanced;
    int left, right;

    if (tree != NULL) // pokud jsme dosli az za listy nebo je strom prazdny
    {
        leftBalanced = isHeightBalanced(tree->left, &left, criticalState); // zjisti, jestli je levy podstrom vyvazeny
        if (!leftBalanced)
        {
            doBalance(&(tree->left), *criticalState); // vyvaz to, co bylo prohlaseno za nevyvazene
            *criticalState = 0;
        }
        rightBalanced = isHeightBalanced(tree->right, &right, criticalState); // zjisti, jestli je pravy podstrom vyvazeny
        if (!rightBalanced)
        {
            doBalance(&(tree->right), *criticalState); // vyvaz to, co bylo prohlaseno za nevyvazene
            *criticalState = 0;
        }


        if (left > right) // do count uloz vysku vetsiho podstromu
        {
            *count = left + 1;
        }
        else
        {
            *count = right + 1;
        }


        int countL = 0;
        int countR = 0;
        if ((left - right) >= 2) // jestli se podstromu ve vysce lisi alespon o 2 a levy je vetsi
        {
            // neni vyvazeny, vlevo tezsi
            if (tree->left->left != NULL)
            {
                isHeightBalanced(tree->left->left, &countL, criticalState); // pro spocitani vysky leveho postromu
            }
            if (tree->left->right != NULL)
            {
                isHeightBalanced(tree->left->right, &countR, criticalState); // pro spocitani vysky praveho podstromu
            }

            if (countL > countR) // urceni situace podle porovnani vysek porstromu
            {
                // LL
                *criticalState = 1;
            }
            else
            {
                // LR
                *criticalState = 2;
            }
        }
        else if ((left - right) <= -2) // jestli se lisi alespon o 2 a pravy je vetsi
        {
            // neni vyvazeny, vpravo tezsi
            if (tree->right->left != NULL)
            {
                isHeightBalanced(tree->right->left, &countL, criticalState);
            }
            if (tree->right->right != NULL)
            {
                isHeightBalanced(tree->right->right, &countR, criticalState);
            }

            if (countL > countR) // porovname vysky synu a urcime situaci
            {
                // RL
                *criticalState = 3;
            }
            else
            {
                // RR
                *criticalState = 4;
            }
        }

        return (leftBalanced && rightBalanced && (abs(left - right) <= 1)); // pokud je vse vybalancovany vrati true, jinak false
    }
    *count = 0; // kdyz podstom je NULL vrati 0 a true
    return true;
}

/** Funkce vyvazi strom */
void balanceTree(Node **tree)
{
    int count, criticalState;
    while (!isHeightBalanced(*tree, &count, &criticalState)) // dokud neni vyvazeny, opakuj
    {
        // neni vyvazeny koren, vyvaz
        doBalance(tree, criticalState);
        criticalState = 0;
    }
}
