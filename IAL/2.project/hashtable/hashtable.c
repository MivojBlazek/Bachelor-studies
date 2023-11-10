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
    (*table)[i] = NULL; // vsechny prvky v tabulce nastav na NULL
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  if ((*table)[get_hash(key)] == NULL) // pokud v tabulce neni ani zaznam s timto klicem, vrat NULL
  {
    return NULL;
  }
  ht_item_t *tmp = (*table)[get_hash(key)];
  while (tmp->key != key) // projed cely seznam prvku se stejnym klicem
  {
    tmp = tmp->next;
    if (tmp == NULL) // kdyz dojedes na konec, vrat NULL
    {
      return NULL;
    }
  }
  return tmp; // jinak prvek byl nalezen
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
  if (search != NULL) // pokud tam uz je, zmen mu hodnotu na novou
  {
    search->value = value;
    return;
  }
  ht_item_t *newItem = (ht_item_t *)malloc(sizeof(ht_item_t)); // jinak vytvor novy prvek
  if (newItem == NULL)
  {
    //! malloc failed
    return;
  }
  newItem->key = key;
  newItem->value = value;
  newItem->next = (*table)[get_hash(key)]; // vloz ho do tabulky podle klice na zacatek seznamu
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
  if (search != NULL) // pokud v tabulce je, vrat jeho hodnotu
  {
    return &(search->value);
  }
  return NULL; // jinak vrat NULL
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
  if (tmp == NULL) // jestli neni zaznam s hledanym hashem, nic nemaz (ani nemas co mazat)
  {
    return;
  }
  while (tmp->key != key) // projdi seznam, dokud ho nenajdes
  {
    beforeTmp = tmp;
    tmp = tmp->next;
    if (tmp == NULL) // pokud jsi na konci, zadny takovy prvek neni, nic nemaz
    {
      return;
    }
  }
  if ((*table)[get_hash(key)] == tmp) // pokud to byl prvni prvek v seznamu
  {
    (*table)[get_hash(key)] = tmp->next; // zmen ukazatel na druhy a smaz ho
    free(tmp);
  }
  else
  {
    beforeTmp->next = tmp->next; // kdyz to nebyl prvni prvek, propoj zpatky seznam a prvek smaz
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
  for (int i = 0; i < HT_SIZE; i++) // pro vsechny radky tabulky kdyz nejsou prazdne
  {
    if ((*table)[i] != NULL)
    {
      ht_item_t *current = (*table)[i];
      ht_item_t *currentNext;
      while (current != NULL) // kazdy radek projed jako seznam dokud nebude prazdny
      {
        currentNext = current->next; // vzdy smaz prvni a posun seznam
        free(current);
        current = currentNext;
      }
    }
    (*table)[i] = NULL; // nastav radky na NULL jako po inicializaci
  }
}
