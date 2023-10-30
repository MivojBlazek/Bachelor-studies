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
    for (int i = 0; (c = input[i]) != '\0'; i++) // dokud neprecteme cely retezec v input
    {
        if (c >= 'A' && c <= 'Z') // pokud to je velke pismeno, udelame z nej male
        {
            c += 'a' - 'A';
        }
        else if (!(c == ' ') && !(c >= 'A' && c <= 'Z') && !(c >= 'a' && c <= 'z'))
        {
            c = '_'; // pokud to neni mezera nebo pismeno, je to podtrzitko
        }

        int value = 0;
        bst_search(*tree, c, &value); // jestli tento znak uz existuje, pricti 1 a zmen mu ve strome hodnotu
        bst_insert(tree, c, ++value);
    }
}

/**
 * Funkce provede dostatečný počet správných rotací
 * 
 * @param tree je ukazatel na kritický uzel
 * @param situation udává situaci která nastala
 * 
 * @returns upravený strom
*/
void balanceTree(bst_node_t **tree, int situation)
{
    bst_node_t *tmp;
    bst_node_t *tmpSon;
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

    if (tree != NULL) // pokud jsme dosli az za listy nebo je strom prazdny
    {
        leftBalanced = isHeightBalanced(tree->left, &left, criticalState); // zjisti, jestli je levy podstrom vyvazeny
        if (!leftBalanced)
        {
            balanceTree(&(tree->left), *criticalState); // vyvaz to, co bylo prohlaseno za nevyvazene
            *criticalState = 0;
        }
        rightBalanced = isHeightBalanced(tree->right, &right, criticalState); // zjisti, jestli je pravy podstrom vyvazeny
        if (!rightBalanced)
        {
            balanceTree(&(tree->right), *criticalState); // vyvaz to, co bylo prohlaseno za nevyvazene
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
    while (!isHeightBalanced(*tree, &count, &criticalState)) // dokud neni vyvazeny, opakuj
    {
        // neni vyvazeny koren, vyvaz
        balanceTree(tree, criticalState);
        criticalState = 0;
    }
}
