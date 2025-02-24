/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	list->activeElement = NULL;
	while (list->firstElement != NULL)
	{
		DLLElementPtr tmpElem = list->firstElement->nextElement; //save second element, free first and make the saved one first (shift by 1 element)
		free(list->firstElement);
		list->firstElement = tmpElem;
		if (list->firstElement == NULL)
		{
			list->lastElement = NULL; //all elements are cleared
		}
		else
		{
			list->firstElement->previousElement = NULL;
		}
	}
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr newElem = malloc(sizeof(struct DLLElement)); //malloc space for new element
	if (newElem == NULL)
	{
		DLL_Error();
		return;
	}
	newElem->data = data;
	newElem->previousElement = NULL;
	newElem->nextElement = list->firstElement; //save new element before the first one and set pointers between elements
	if (list->firstElement == NULL)
	{
		list->lastElement = newElem;
	}
	else
	{
		newElem->nextElement->previousElement = newElem;
	}
	list->firstElement = newElem; //make our new element the first in list
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	DLLElementPtr newElem = malloc(sizeof(struct DLLElement)); //malloc space for new element
	if (newElem == NULL)
	{
		DLL_Error();
		return;
	}
	newElem->data = data;
	newElem->nextElement = NULL;
	newElem->previousElement = list->lastElement; //save new element after the last one and set pointers between elements
	if (list->lastElement == NULL)
	{
		list->firstElement = newElem;
	}
	else
	{
		newElem->previousElement->nextElement = newElem;
	}
	list->lastElement = newElem; //make our new element last in list
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if (list->firstElement == NULL)
	{
		DLL_Error();
		return;
	}
	*dataPtr = list->firstElement->data; //take data from first element in list
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	if (list->lastElement == NULL)
	{
		DLL_Error();
		return;
	}
	*dataPtr = list->lastElement->data; //take data from last element in list
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (list->firstElement == NULL)
	{
		return;
	}
	if (list->activeElement == list->firstElement)
	{
		list->activeElement = NULL;
	}
	DLLElementPtr tmpElem = list->firstElement->nextElement; //temporarily save second element and free the first one
	free(list->firstElement);
	list->firstElement = tmpElem; //save the tmpElem as first in list and set pointers
	if (list->firstElement == NULL)
	{
		list->lastElement = NULL;
	}
	else
	{
		list->firstElement->previousElement = NULL;
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if (list->lastElement == NULL)
	{
		return;
	}
	if (list->activeElement == list->lastElement)
	{
		list->activeElement = NULL;
	}
	DLLElementPtr tmpElem = list->lastElement->previousElement;//temporarily save second element from end and free the last one
	free(list->lastElement);
	list->lastElement = tmpElem; //save the tmpElem as last one in list and set pointers
	if (list->lastElement == NULL)
	{
		list->firstElement = NULL;
	}
	else
	{
		list->lastElement->nextElement = NULL;
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if (list->activeElement != NULL && list->activeElement != list->lastElement)
	{
		DLLElementPtr tmpElem = list->activeElement->nextElement; //temporarily save element after active and shift right part of list, so we have new list without tmpElem and tmpElem
		list->activeElement->nextElement = tmpElem->nextElement;
		if (tmpElem->nextElement != NULL)
		{
			tmpElem->nextElement->previousElement = list->activeElement;
		}
		else
		{
			list->lastElement = list->activeElement;
		}
		free(tmpElem); //free deleted element
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	if (list->activeElement != NULL && list->activeElement != list->firstElement)
	{
		DLLElementPtr tmpElem = list->activeElement->previousElement; //temporarily save element before active and left right part of list, so we have new list without tmpElem and tmpElem
		list->activeElement->previousElement = tmpElem->previousElement;
		if (tmpElem->previousElement != NULL)
		{
			tmpElem->previousElement->nextElement = list->activeElement;
		}
		else
		{
			list->firstElement = list->activeElement;
		}
		free(tmpElem); //free deleted element
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	if (list->activeElement != NULL)
	{
		DLLElementPtr tmpElem = list->activeElement->nextElement; //save element after active and malloc a new one instead of it
		list->activeElement->nextElement = malloc(sizeof(struct DLLElement));
		if (list->activeElement->nextElement == NULL)
		{
			DLL_Error();
			return;
		}
		list->activeElement->nextElement->nextElement = tmpElem; //then put these 2 lists together and set pointers between them
		list->activeElement->nextElement->previousElement = list->activeElement;
		list->activeElement->nextElement->data = data; //of course save data value to new element
		if (tmpElem != NULL)
		{
			tmpElem->previousElement = list->activeElement->nextElement;
		}
		else
		{
			list->lastElement = list->activeElement->nextElement;
		}
	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	if (list->activeElement != NULL)
	{
		DLLElementPtr tmpElem = list->activeElement->previousElement; //save element before active and malloc a new one instead of it
		list->activeElement->previousElement = malloc(sizeof(struct DLLElement));
		if (list->activeElement->previousElement == NULL)
		{
			DLL_Error();
			return;
		}
		list->activeElement->previousElement->previousElement = tmpElem; //then put these 2 lists together and set pointers between them
		list->activeElement->previousElement->nextElement = list->activeElement;
		list->activeElement->previousElement->data = data; //of course save data value to new element
		if (tmpElem != NULL)
		{
			tmpElem->nextElement = list->activeElement->previousElement;
		}
		else
		{
			list->firstElement = list->activeElement->previousElement;
		}
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if (list->activeElement == NULL)
	{
		DLL_Error();
		return;
	}
	*dataPtr = list->activeElement->data; //save active elements data to dataPtr
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if (list->activeElement != NULL)
	{
		list->activeElement->data = data;
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement != NULL && list->activeElement != list->lastElement)
	{
		list->activeElement = list->activeElement->nextElement;
	}
	else
	{
		list->activeElement = NULL; //if last element was active -> nothing is active
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (list->activeElement != NULL && list->activeElement != list->firstElement)
	{
		list->activeElement = list->activeElement->previousElement;
	}
	else
	{
		list->activeElement = NULL; //if first element was active -> nothing is active
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return list->activeElement == NULL ? 0 : 1;
}

/* Konec c206.c */
