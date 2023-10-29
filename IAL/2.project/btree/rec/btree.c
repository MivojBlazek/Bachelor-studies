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
  (*tree) = NULL;
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
    if (tree->key == key) // kdyz jsme klic nasli, vrat hodnotu a true
    {
      *value = tree->value;
      return true;
    }
    else if (key < tree->key) // kdyz je mensi nez nalezeny, rekurzivne se podivej do leveho podstromu
    {
      return bst_search(tree->left, key, value);
    }
    else // kdyz je vetsi nez nalezeny, rekurzivne se podivej do praveho podstromu
    {
      return bst_search(tree->right, key, value);
    }
  }
  return false; // pokud dojedu do listu na ukazatel na NULL, nenasel jsem ho
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
  if ((*tree) == NULL) // pokud neexistuje zadny uzel nebo jsme dosli az pod list na misto kam patri, vytvor novy
  {
    (*tree) = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (*tree == NULL)
    {
      //! malloc failed
      return;
    }
    
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    (*tree)->key = key;
    (*tree)->value = value;
  }
  else // jiank se podivej dal ve strome doleva nebo doprava podle hodnoty klice
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
      (*tree)->value = value; // nasli jsme uzel se stejnym klicem, zmen mu hodnotu
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
  bst_node_t *tmp;
  if ((*tree)->right == NULL) // kdyz uz vic vpravo neni zadny uzel
  {
    target->key = (*tree)->key; // uloz hodnoty nejpravejsiho do target
    target->value = (*tree)->value;
    if ((*tree)->left != NULL) // a kdyz to neni list (ma leveho syna)
    {
      tmp = (*tree)->left; // posun syna na jeho pozici a smaz toho syna (zachovej ukazatele dal)
      free(*tree);
      *tree = tmp;
    }
    else // a kdyz je to list, vymaz ho
    {
      free(*tree);
      *tree = NULL;
    }
  }
  else // pokud jeste muzeme jit doprava, jdeme tam
  {
    bst_replace_by_rightmost(target, &(*tree)->right);
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
    if (key < (*tree)->key) // kdyz je v levem podstromu, prohledej ten, pokud existuje
    {
      if ((*tree)->left == NULL)
      {
        return;
      }
      bst_delete(&(*tree)->left, key);
    }
    else if (key > (*tree)->key) // kdyz je v pravem podstromu, prohledej ten, pokud existuje
    {
      if ((*tree)->right == NULL)
      {
        return;
      }
      bst_delete(&(*tree)->right, key);
    }
    else // kdyz jsme ho nasli
    {
      if ((*tree)->left == NULL && (*tree)->right == NULL) // a nema syny, tak ho smazeme
      {
        free(*tree);
        *tree = NULL;
      }
      else if ((*tree)->left != NULL && (*tree)->right != NULL) // a ma oba syny, tak ho nahradime nejpravejsim uzel z jeho leveho podstromu
      {
        bst_replace_by_rightmost(*tree, &(*tree)->left);
      }
      else // a kdyz ma jen jednoho syna
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
        (*tree)->left = tmp->left; // smazeme uzel toho syna posuneme na jeho misto
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
  if (*tree == NULL) // kdzy uz nejsou uzly, koncime
  {
    return;
  }
  bst_dispose(&((*tree)->left)); // rekurzivne promazeme levy podstrom
  bst_dispose(&((*tree)->right)); // a pote i pravy podstrom
  free(*tree); // nakonec smazeme i koren
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
    bst_add_node_to_items(tree, items); // vzdy zapis uzel a az pak jdi dal (nejdrive do leveho a az se sem vratime, tak i do praveho)
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
    bst_inorder(tree->left, items); // vzdy jdi dal do leveho uzlu pokud muzes a az potom ho zapis, pak projdi i pravy podstrom
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
    bst_postorder(tree->left, items); // vzdy jdi dal do leveho podstromu pokud muzes a potom projdi i pravy podstrom pokud muzes, nakonec zapis uzel
    bst_postorder(tree->right, items);
    bst_add_node_to_items(tree, items);
  }
}
