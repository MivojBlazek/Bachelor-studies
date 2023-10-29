/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++)
  {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  if ((*table)[get_hash(key)] == NULL)
  {
    return NULL;
  }
  ht_item_t *tmp = (*table)[get_hash(key)];
  while (tmp->key != key)
  {
    tmp = tmp->next;
    if (tmp == NULL)
    {
      return NULL;
    }
  }
  return tmp;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  ht_item_t *search = ht_search(table, key);
  if (search != NULL)
  {
    search->value = value;
    return;
  }
  ht_item_t *newItem = (ht_item_t *)malloc(sizeof(ht_item_t));
  newItem->key = key;
  newItem->value = value;
  newItem->next = (*table)[get_hash(key)];
  (*table)[get_hash(key)] = newItem;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *search = ht_search(table, key);
  if (search != NULL)
  {
    return &(search->value);
  }
  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  ht_item_t *beforeTmp;
  ht_item_t *tmp = (*table)[get_hash(key)];
  if (tmp == NULL)
  {
    return;
  }
  while (tmp->key != key)
  {
    beforeTmp = tmp;
    tmp = tmp->next;
    if (tmp == NULL)
    {
      return;
    }
  }
  if ((*table)[get_hash(key)] == tmp)
  {
    (*table)[get_hash(key)] = tmp->next;
    free(tmp);
  }
  else
  {
    beforeTmp->next = tmp->next;
    free(tmp);
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++)
  {
    if ((*table)[i] != NULL)
    {
      ht_item_t *current = (*table)[i];
      ht_item_t *currentNext;
      while (current != NULL)
      {
        currentNext = current->next;
        free(current);
        current = currentNext;
      }
    }
    (*table)[i] = NULL;
  }
}
