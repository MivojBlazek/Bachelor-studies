/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  *tree = (bst_node_t *)malloc(sizeof(bst_node_t));
  if ((*tree) == NULL)
  {
    //! malloc failed
    return;
  }
  (*tree)->key = 0;
  (*tree)->value = 0;
  (*tree)->left = NULL;
  (*tree)->right = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *tmp = tree;
  while (tmp != NULL)
  {
    if (tmp->key > key)
    {
      tmp = tmp->left;
    }
    else if (tmp->key < key)
    {
      tmp = tmp->right;
    }
    else
    {
      *value = tmp->value;
      return true;
    }
  }
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if ((*tree) == NULL)
  {
    bst_init(tree);
    (*tree)->key = key;
    (*tree)->value = value;
    return;
  }

  bst_node_t *tmp = *tree;
  bst_node_t *newNode;
  while (tmp != NULL)
  {
    if (tmp->key > key)
    {
      if (tmp->left == NULL)
      {
        bst_init(&newNode);
        newNode->key = key;
        newNode->value = value;
        tmp->left = newNode;
        return;
      }
      tmp = tmp->left;
    }
    else if (tmp->key < key)
    {
      if (tmp->right == NULL)
      {
        bst_init(&newNode);
        newNode->key = key;
        newNode->value = value;
        tmp->right = newNode;
        return;
      }
      tmp = tmp->right;
    }
    else
    {
      tmp->value = value;
      return;
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  target = *tree;
  while (target != NULL)
  {
    if (target->right != NULL)
    {
      target = target->right;
    }
  }
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *tmp = *tree;
  bst_node_t *beforeTmp;
  int left;
  while (tmp != NULL)
  {
    if (key < tmp->key)
    {
      if (tmp->left == NULL)
      {
        return;
      }
      beforeTmp = tmp;
      left = 1;
      tmp = tmp->left;
    }
    else if (key > tmp->key)
    {
      if (tmp->right == NULL)
      {
        return;
      }
      beforeTmp = tmp;
      left = 0;
      tmp = tmp->right;
    }
    else
    {
      if (tmp->left == NULL && tmp->right == NULL)
      {
        free(tmp);
        tmp = NULL;
        if (left)
        {
          beforeTmp->left = NULL;
        }
        else
        {
          beforeTmp->right = NULL;
        }
      }
      else if (tmp->left != NULL && tmp->right != NULL)
      {
        bst_replace_by_rightmost(tmp, &tmp->left);
      }
      else
      {
        if (tmp->left == NULL)
        {
          tmp = tmp->right;
        }
        else
        {
          tmp = tmp->left;
        }
        beforeTmp->left = tmp->left;
        beforeTmp->right = tmp->right;
        beforeTmp->value = tmp->value;
        beforeTmp->key = tmp->key;
        free(tmp);
        tmp = NULL;
      }
    }
  }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  for (int i = 1; i < 25; i++)
  {
    bst_delete(tree, (char)i); //! nejak podle klice mazat
  }
  (*tree)->key = 0; //! mby overkill 4 radky navic
  (*tree)->value = 0;
  (*tree)->left = NULL;
  (*tree)->right = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
}
