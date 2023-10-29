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
  (*tree) = NULL;
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
  while (tmp != NULL) // dokud jsme nenarazili na NULL za nejakym listem
  {
    if (tmp->key > key) // prohledavej levy podstrom
    {
      tmp = tmp->left;
    }
    else if (tmp->key < key) // prohledavej pravy podstrom
    {
      tmp = tmp->right;
    }
    else // nasel jsi, vrat jeho hodnotu a true
    {
      *value = tmp->value;
      return true;
    }
  }
  return false; // pokud jsme narazili na NULL a nenasli jsme prvek, vrat false
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
  if ((*tree) == NULL) // jestlize je strom pouze inicializovany, vytvor novy uzel
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
    return;
  }

  bst_node_t *tmp = *tree;
  bst_node_t *newNode;
  while (tmp != NULL) // jiank prohledavej strom dokud nebudes az za nejakym listem
  {
    if (tmp->key > key) // pokud je vetsi, prohledej levy podstrom
    {
      if (tmp->left == NULL) // ale kdyz zadny podstrom neni, tak tam vytvor novy prvek
      {
        newNode = (bst_node_t *)malloc(sizeof(bst_node_t));
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->key = key;
        newNode->value = value;

        tmp->left = newNode;
        return;
      }
      tmp = tmp->left;
    }
    else if (tmp->key < key)// pokud je mensi, prohledej pravy podstrom
    {
      if (tmp->right == NULL) // ale kdyz zadny podstrom neni, tak tam vytvor novy prvek
      {
        newNode = (bst_node_t *)malloc(sizeof(bst_node_t));
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->key = key;
        newNode->value = value;

        tmp->right = newNode;
        return;
      }
      tmp = tmp->right;
    }
    else // kdyz jsme nasli prvek se stejnym klicem, jen prepis jeho hodnotu
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
  bst_node_t *tmp = *tree;
  bst_node_t *beforeTmp = target;
  while (tmp != NULL) // projizdej stromem dokud nenajdes nejpravejsi uzel
  {
    if (tmp->right != NULL) // pokud existuje pravejsi uzel, prozkoumej to
    {
      beforeTmp = tmp;
      tmp = tmp->right;
    }
    else // nasli jsme nejpravejsi uzel
    {
      target->key = tmp->key; // ulozime jeho hodnoty do target
      target->value = tmp->value;
      if (beforeTmp == target)
      {
        target->left = tmp->left; // posuneme zbytek podstomu pod nejpravejsim uzlem misto nej
      }
      else
      {
        beforeTmp->right = tmp->left;
      }
      free(tmp); // smazeme okopirovany uzel
      tmp = NULL;
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
  int left = -1;
  while (tmp != NULL) // dokud nejsme za listem na konci nejakeho podstomu
  {
    if (key < tmp->key) // pokud je hledany uzel v levem podstromu a tento podstrom existuje, prohledej ho
    {
      if (tmp->left == NULL)
      {
        return;
      }
      beforeTmp = tmp;
      left = 1;
      tmp = tmp->left;
    }
    else if (key > tmp->key) // pokud je hledany uzel v pravem podstromu a tento podstrom existuje, prohledej ho
    {
      if (tmp->right == NULL)
      {
        return;
      }
      beforeTmp = tmp;
      left = 0;
      tmp = tmp->right;
    }
    else // pokud jsme nasli hledany uzel
    {
      if (tmp->left == NULL && tmp->right == NULL) // a nema ani jednoho syna
      {
        free(tmp);
        tmp = NULL; // smazeme ho a kdyz je to koren, nastavime *tree na NULL
        if (left == -1)
        {
          *tree = NULL;
          return;
        }
        if (left) // kdyz je v levem nebo pravem podstromu, nastavime spravne ukazatele, aby se strom propojil
        {
          beforeTmp->left = NULL;
        }
        else
        {
          beforeTmp->right = NULL;
        }
      }
      else if (tmp->left != NULL && tmp->right != NULL) // a ma oba syny
      {
        bst_replace_by_rightmost(tmp, &tmp->left); // nahradime ho nejpravejsim uzlem v jeho levem podstromu
        return;
      }
      else // a ma jen 1 syna
      {
        beforeTmp = tmp;
        if (tmp->left == NULL) // urcime, jestli to je levy nebo pravy
        {
          tmp = tmp->right;
        }
        else
        {
          tmp = tmp->left;
        }
        beforeTmp->left = tmp->left; // vhodne propojime strom bez tohoto uzlu a tento uzel smazeme
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
  if (*tree == NULL) // pokud nemame co mazat, nemazeme
  {
    return;
  }
  stack_bst_t stack;
  stack_bst_init(&stack); // inicializace zasobniku
  bst_node_t *tmp = *tree;
  bst_node_t *lastVisited = NULL;

  while (tmp || stack.top != -1) // dokud mame neco v zasobniku nebo jsme jeste nedosli na konec stromu
  {
    if (tmp) // kdyz jsme v nejakem uzlu, hodime ho na zasobnik a presuneme se o 1 doleva
    {
      stack_bst_push(&stack, tmp);
      tmp = tmp->left;
    }
    else // kdyz uz nejsme v zadnem uzlu, vyprazdnujeme zasobnik
    {
      bst_node_t *tmp2 = stack_bst_top(&stack);
      if (tmp2->right && tmp2->right != lastVisited) // koukneme co je na vrsku zasobnik a kdyz jeho pravy syn neni naposled navstiven, posuneme se doprava od tohoto uzlu
      {
        tmp = tmp2->right; // jednoduse z kazdeho leveho uzlu zkousime jit doprava a mazeme
      }
      else // jinak tento uzel prenastavime na posledni navstiven, smazeme ho a vyndame ho ze zasobniku
      {
        lastVisited = tmp2;
        free(tmp2);
        stack_bst_pop(&stack);
      }
    }
  }
  *tree = NULL; // nastavime strom jako po inicializaci
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
  while (tree != NULL) // dokud jsme nedosli v levem pruchodu na konec stromu
  {
    stack_bst_push(to_visit, tree); // vloz vsechny leve uzly na zasobnik a zapis je
    bst_add_node_to_items(tree, items);
    tree = tree->left;
  }
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
  stack_bst_t stack;
  stack_bst_init(&stack); // inicializace zasobniku
  bst_leftmost_preorder(tree, &stack, items); // naplneni zasobniku levymi uzly

  while (!stack_bst_empty(&stack)) // dokud nevyprazdnime zasobnik
  {
    tree = stack_bst_pop(&stack); // vkladame na zasobnik jeste pravy uzel kazdeho naseho leveho az projdeme cely strom
    bst_leftmost_preorder(tree->right, &stack, items);
  }
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
  while (tree != NULL) // dokud nejsme na konci stromu
  {
    stack_bst_push(to_visit, tree); // vkladej leve uzly na zasobnik
    tree = tree->left;
  }
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
  stack_bst_t stack;
  stack_bst_init(&stack); // inicializace zasobniku
  bst_leftmost_inorder(tree, &stack); // naplneni zasobniku levymi uzly

  while (!stack_bst_empty(&stack)) // dokud zasobnik neni prazdny
  {
    tree = stack_bst_pop(&stack); // vkladame jako u preorderu na zasobnik i prave uzly, ale prubezne zapisujeme po zpracovani leveho podstromu
    bst_add_node_to_items(tree, items);
    bst_leftmost_inorder(tree->right, &stack);
  }
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
  while (tree != NULL) // dokud nejsme na konci stromu
  {
    stack_bst_push(to_visit, tree); // vkladame na zasobnik leve uzly
    stack_bool_push(first_visit, true); // a na zasobnik navstivenosti davame true
    tree = tree->left;
  }
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
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bool_t stackBool;
  stack_bool_init(&stackBool); // inicializace obou zasobniku
  bool fromLeft;

  bst_leftmost_postorder(tree, &stack, &stackBool); // naplneni zasobniku levymi uzly
  while (!stack_bst_empty(&stack)) // dokud neni zasobnik uzlu prazdny
  {
    tree = stack_bst_top(&stack);
    fromLeft = stack_bool_pop(&stackBool); // zjistime, jestli jdeme zleva
    if (fromLeft) // pokud ano
    {
      stack_bool_push(&stackBool, false); // prepiseme na false a klasicky pridame na zasobnik pravy uzel
      bst_leftmost_postorder(tree->right, &stack, &stackBool);
    }
    else // pokud nejdeme zleva (tzn je zpracovano vse v obou podstromech)
    {
      stack_bst_pop(&stack); // uz tento uzel nepotrebujeme a zapiseme ho
      bst_add_node_to_items(tree, items);
    }
  }
}
