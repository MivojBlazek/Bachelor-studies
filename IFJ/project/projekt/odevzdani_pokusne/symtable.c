/**
 * @file symtable.c
 * 
 * IFJ Projekt 2023
 * 
 * @author <xblaze38> Michal Blažek
 * @author <xmicha94> Kryštof Michálek
 * @author <xsapik02> Matyáš Sapík
 * @author <xzavod16> Ondřej Závodský
*/

#include "symtable.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** Promenna urcujici prepsani v tabulce symbolu pri insertNode() */
bool changing;

/** Promenna zachytavajici errory */
int errorT = 0;

/** Inicializace stromu */
void initTree(Node **tree)
{
    (*tree) = NULL;
}

/** Funkce najde uzel podle jmena, jinak vraci NULL */
Node *searchNode(Node *tree, char *name)
{
    if (tree)
    {
        if (!strcmp(tree->name, name))
        {
            return tree;
        }
        else if (strcmp(name, tree->name) < 0) //! mby obracena podminka
        {
            return searchNode(tree->left, name);
        }
        else
        {
            return searchNode(tree->right, name);
        }
    }
    return NULL;
}

/** Pomocna funkce pro najezeni nejpravejsiho listu v levem podstromu */
Node *findRightmost(Node **tree)
{
    if ((*tree)->right == NULL)
    {
        return (*tree);
    }
    else
    {
        return findRightmost(&((*tree)->right));
    }
}

/** Pomocna funkce, ktera vlozi do stromu novy uzel */
void insertNodeWithoutBalance(Node **tree, Node *insert)
{
    changing = false;
    if ((*tree) == NULL)
    {
        (*tree) = (Node *)malloc(sizeof(Node));
        if (*tree == NULL)
        {
            errorT = 1;
            return; //! malloc failed
        }
        (*tree)->params = malloc(255 * sizeof(Param));

        (*tree)->left = NULL;
        (*tree)->right = NULL;
        (*tree)->name = insert->name;
        (*tree)->type = insert->type;
        (*tree)->func = insert->func;
        (*tree)->const_let = insert->const_let;
        for (int i = 0; i < 255; i++)
        {
            if (insert->params[i].name == NULL)
            {
                return;
            }
            (*tree)->params[i] = insert->params[i];
        }
    }
    else
    {
        if (strcmp(insert->name, (*tree)->name) < 0)
        {
            insertNode(&(*tree)->left, insert);
        }
        else if (strcmp(insert->name, (*tree)->name) > 0)
        {
            insertNode(&(*tree)->right, insert);
        }
        else // nalezen stejny klic
        {
            if ((*tree)->func == insert->func) // nalezen stejny klic stejneho typu
            {
                changing = true;
                (*tree)->type = insert->type;
                for (int i = 0; i < 255; i++)
                {
                    if ((insert->params)[i].name == NULL)
                    {
                        return;
                    }
                    ((*tree)->params)[i] = (insert->params)[i];
                }
            }
            else // nalezen stejny klic jineho typu
            {
                Node *tmp = searchNode((*tree)->left, insert->name);
                if (tmp == NULL)
                {
                    tmp = searchNode((*tree)->right, insert->name);
                }

                if (tmp != NULL) // stejny klic a stejny typ
                {
                    changing = true;
                    (*tree)->type = insert->type;
                    for (int i = 0; i < 255; i++)
                    {
                        if ((insert->params)[i].name == NULL)
                        {
                            return;
                        }
                        ((*tree)->params)[i] = (insert->params)[i];
                    }
                }
                else // stejny klic a jiny typ
                {
                    if ((*tree)->left == NULL)
                    {
                        (*tree)->left = (Node *)malloc(sizeof(Node));
                        if ((*tree)->left == NULL)
                        {
                            errorT = 1;
                            return; //! malloc failed
                        }
                        (*tree)->params = malloc(255 * sizeof(Param));
                        
                        (*tree)->left->left = NULL;
                        (*tree)->left->right = NULL;
                        (*tree)->left->name = insert->name;
                        (*tree)->left->type = insert->type;
                        (*tree)->left->func = insert->func;
                        (*tree)->const_let = insert->const_let;
                        for (int i = 0; i < 255; i++)
                        {
                            if ((insert->params)[i].name == NULL)
                            {
                                return;
                            }
                            ((*tree)->left->params)[i] = (insert->params)[i];
                        }
                    }
                    else
                    {
                        tmp = findRightmost(&(*tree)->left);
                        tmp->right = (Node *)malloc(sizeof(Node));
                        if (tmp->right == NULL)
                        {
                            errorT = 1;
                            return; //! malloc failed
                        }
                        (*tree)->params = malloc(255 * sizeof(Param));
                        
                        tmp->right->left = NULL;
                        tmp->right->right = NULL;
                        tmp->right->name = insert->name;
                        tmp->right->type = insert->type;
                        tmp->right->func = insert->func;
                        (*tree)->const_let = insert->const_let;
                        for (int i = 0; i < 255; i++)
                        {
                            if ((insert->params)[i].name == NULL)
                            {
                                return;
                            }
                            (tmp->right->params)[i] = (insert->params)[i];
                        }
                    }
                }
            }
        }
    }
}

/** Funkce vycisti cely strom */
void disposeTree(Node **tree)
{
    if (*tree != NULL)
    {
        disposeTree(&((*tree)->left));
        disposeTree(&((*tree)->right));
        // free((*tree)->name);
        free(*tree);
        *tree = NULL;
    }
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

/** Funkce vlozi do stromu novy uzel a vyskove vyvazi strom */
void insertNode(Node **tree, Node *insert)
{
    insertNodeWithoutBalance(tree, insert);
    balanceTree(tree);
}
