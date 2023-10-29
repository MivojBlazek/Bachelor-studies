/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree);
    char c;
    for (int i = 0; (c = input[i]) != '\0'; i++)
    {
        if (c >= 'A' && c <= 'Z')
        {
            c += 'a' - 'A';
        }
        else if (!(c == ' ') && !(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z'))
        {
            c = '_';
        }

        int value = 0;
        bst_search(*tree, c, &value);
        bst_insert(tree, c, ++value);
    }
}

/**
 * Funkce provede dostatečný počet správných rotací
 * 
 * @param tree je ukazatel na kriticky uzel
 * @param situation udava situaci ktera nastala
 * 
 * @returns upraveny strom
*/
void balanceTree(bst_node_t **tree, int situation)
{
    bst_node_t *tmp;
    bst_node_t *tmpSon;
    switch (situation) // LL, LR, RL, RR
    {
        case 1:
            tmp = (*tree)->left->right;
            tmpSon = (*tree)->left;
            (*tree)->left->right = *tree;
            (*tree)->left = tmp;
            break;

        case 2:
            // 1. rotace
            tmp = (*tree)->left->right->left;
            tmpSon = (*tree)->left->right;
            (*tree)->left->right->left = (*tree)->left;
            (*tree)->left->right = tmp;
            (*tree)->left = tmpSon;

            // 2. rotace
            tmp = (*tree)->left->right;
            tmpSon = (*tree)->left;
            (*tree)->left->right = *tree;
            (*tree)->left = tmp;
            break;

        case 3:
            // 1. rotace
            tmp = (*tree)->right->left->right;
            tmpSon = (*tree)->right->left;
            (*tree)->right->left->right = (*tree)->right;
            (*tree)->right->left = tmp;
            (*tree)->right = tmpSon;

            // 2. rotace
            tmp = (*tree)->right->left;
            tmpSon = (*tree)->right;
            (*tree)->right->left = *tree;
            (*tree)->right = tmp;
            break;

        case 4:
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
 * Funkce zjistí, jestli binarní strom je výškově vyvážený
 * 
 * @param tree očekává strom pro který bude zjišťovat vyváženost
 * @param count vrací výšku stromu
 * 
 * @returns true - pokud je vyvážený \n
 * @returns false - pokud není vyvážený
*/
bool isHeightBalanced(bst_node_t *tree, int *count, int *criticalState)
{
    bool leftBalanced, rightBalanced;
    int left, right;

    if (tree != NULL)
    {
        leftBalanced = isHeightBalanced(tree->left, &left, criticalState);
        if (!leftBalanced)
        {
            balanceTree(&(tree->left), *criticalState);
            *criticalState = 0;
        }
        rightBalanced = isHeightBalanced(tree->right, &right, criticalState);
        if (!rightBalanced)
        {
            balanceTree(&(tree->right), *criticalState);
            *criticalState = 0;
        }


        if (left > right)
        {
            *count = left + 1;
        }
        else
        {
            *count = right + 1;
        }


        int countL = 0;
        int countR = 0;
        if ((left - right) >= 2)
        {
            // neni vyvazeny, vlevo tezsi
            if (tree->left->left != NULL)
            {
                isHeightBalanced(tree->left->left, &countL, criticalState);
            }
            if (tree->left->right != NULL)
            {
                isHeightBalanced(tree->left->right, &countR, criticalState);
            }

            if (countL > countR)
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
        else if ((left - right) <= -2)
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

            if (countL > countR)
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

        return (leftBalanced && rightBalanced && (abs(left - right) <= 1));
    }
    *count = 0;
    return true;
}

/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    int count, criticalState;
    while (!isHeightBalanced(*tree, &count, &criticalState)) //while neni true
    {
        //neni vyvazeny
        balanceTree(tree, criticalState);
        criticalState = 0;
    }
}
