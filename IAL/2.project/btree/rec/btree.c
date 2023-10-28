/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
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
  /**tree = (bst_node_t *)malloc(sizeof(bst_node_t));
  if ((*tree) == NULL)
  {
    //! malloc failed
    return;
  }*/
  (*tree) = NULL;
  /*
  (*tree)->key = 0;
  (*tree)->value = 0;
  (*tree)->left = NULL;
  (*tree)->right = NULL;*/
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree)
  {
    if (tree->key == key)
    {
      *value = tree->value;
      return true;
    }
    else if (key < tree->key)
    {
      return bst_search(tree->left, key, value);
    }
    else
    {
      return bst_search(tree->right, key, value);
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if ((*tree) == NULL)
  {
    (*tree) = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (*tree == NULL)
    {
      //! malloc failed
      return;
    }
    
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    // bst_init(tree);
    (*tree)->key = key;
    (*tree)->value = value;
  }
  else
  {
    if (key < (*tree)->key)
    {
      bst_insert(&(*tree)->left, key, value);
    }
    else if (key > (*tree)->key)
    {
      bst_insert(&(*tree)->right, key, value);
    }
    else
    {
      (*tree)->value = value;
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if ((*tree)->right == NULL)
  {
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    (*tree)->key = 0;
  }
  else
  {
    bst_replace_by_rightmost(target, &(*tree)->right);
    if ((*tree)->right->key == 0)
    {
      bst_node_t *tmp = (*tree)->right->left;
      free((*tree)->right);
      (*tree)->right = tmp;
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
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (*tree)
  {
    if (key < (*tree)->key)
    {
      if ((*tree)->left == NULL)
      {
        return;
      }
      bst_delete(&(*tree)->left, key);
      if ((*tree)->left->key == 0)
      {
        free((*tree)->left);
        (*tree)->left = NULL;
      }
    }
    else if (key > (*tree)->key)
    {
      if ((*tree)->right == NULL)
      {
        return;
      }
      bst_delete(&(*tree)->right, key);
      if ((*tree)->right->key == 0)
      {
        free((*tree)->right);
        (*tree)->right = NULL;
      }
    }
    else
    {
      if ((*tree)->left == NULL && (*tree)->right == NULL)
      {
        (*tree)->key = 0;
      }
      else if ((*tree)->left != NULL && (*tree)->right != NULL)
      {
        bst_replace_by_rightmost(*tree, &(*tree)->left);
        if ((*tree)->left->key == 0)
        {
          bst_node_t *tmp = (*tree)->left->left;
          free((*tree)->left);
          (*tree)->left = tmp;
        }
      }
      else
      {
        bst_node_t *tmp;
        if ((*tree)->left == NULL)
        {
          tmp = (*tree)->right;
        }
        else
        {
          tmp = (*tree)->left;
        }
        (*tree)->left = tmp->left;
        (*tree)->right = tmp->right;
        (*tree)->value = tmp->value;
        (*tree)->key = tmp->key;
        free(tmp);
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
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  /*for (int i = 1; i < 256; i++)
  {
    bst_delete(tree, (char)i); //! nejak podle klice mazat
  }
  (*tree)->key = 0; //! mby overkill 4 radky navic
  (*tree)->value = 0;
  (*tree)->left = NULL;
  (*tree)->right = NULL;*/
  /* while (!((*tree)->left == NULL && (*tree)->right == NULL))
  {
    bst_delete(tree, (*tree)->key);
  }
  (*tree)->key = 0;
  (*tree)->value = 0;
  (*tree)->left = NULL;
  (*tree)->right = NULL;*/
  if (*tree == NULL)
  {
    return;
  }
  bst_dispose(&((*tree)->left));
  bst_dispose(&((*tree)->right));
  free(*tree);
  *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL)
  {
    bst_add_node_to_items(tree, items);
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL)
  {
    bst_inorder(tree->left, items);
    bst_add_node_to_items(tree, items);
    bst_inorder(tree->right, items);
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL)
  {
    bst_postorder(tree->left, items);
    bst_postorder(tree->right, items);
    bst_add_node_to_items(tree, items);
  }
}
