/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char tmp;
	Stack_Top(stack, &tmp);
	Stack_Pop(stack);
	while (tmp != '(') //after left bracket stop loading
	{
		postfixExpression[(*postfixExpressionLength)++] = tmp; //save items from stack to postfix string
		Stack_Top(stack, &tmp); //take another item from stack and save it to tmp until left bracket
		Stack_Pop(stack);
	}
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char tmp;
	if (!Stack_IsEmpty(stack))
	{
		Stack_Top(stack, &tmp);
	}
	while (!Stack_IsEmpty(stack) && tmp != '(')
	{
		if ((tmp == '+' || tmp == '-') && (c == '*' || c == '/'))
		{
			break; //push on the stack because we have higher priority operator then top of the stack
		}
		postfixExpression[(*postfixExpressionLength)++] = tmp; //same or higher priority operator on stack -> delete, write to postfix and repeat (while)
		Stack_Pop(stack);
		if (!Stack_IsEmpty(stack)) //read next item from stack
		{
			Stack_Top(stack, &tmp);
		}
	}
	Stack_Push(stack, c);
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	Stack stack;
	Stack_Init(&stack); //init of new stack

	unsigned pos = 0;
	char *postfixExpression = malloc(sizeof(char) * MAX_LEN); //allocate memory for postfix result
	if (postfixExpression == NULL)
	{
		return NULL; //error with malloc
	}

	for (unsigned i = 0; i < MAX_LEN && infixExpression[i] != '\0'; i++)
	{
		char tmp;
		switch (infixExpression[i])
		{
			case '(':
				Stack_Push(&stack, '(');
				break;

			case ')':
				untilLeftPar(&stack, postfixExpression, &pos);
				break;

			case '+':
			case '-':
			case '*':
			case '/':
				doOperation(&stack, infixExpression[i], postfixExpression, &pos);
				break;

			case '=':
				while (!Stack_IsEmpty(&stack)) //write from stack to postfix string char by char
				{
					Stack_Top(&stack, &tmp);
					Stack_Pop(&stack);
					postfixExpression[pos++] = tmp;
				}
				postfixExpression[pos++] = '=';
				break;

			default: //operand
				postfixExpression[pos++] = infixExpression[i];
				break;
		}
	}
	postfixExpression[pos++] = '\0';
	Stack_Dispose(&stack);
	return postfixExpression;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
	char *value_converted = (char *)(&value); //save int value to char *
	for (int i = (sizeof(int)/sizeof(char)) - 1; i >= 0; i--) //store all 4 bytes to stack starting with LSB and ending with MSB, so MSB will be on top
	{
		Stack_Push(stack, value_converted[i]);
	}
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
	for (int i = 0; i < (int)(sizeof(int)/sizeof(char)); i++)
	{
		Stack_Top(stack, (char *)((char *)value + i)); //save to value and its i-th byte
		Stack_Pop(stack);
	}
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	Stack stack;
	Stack_Init(&stack); //init of new stack
	
	int val1, val2;
	char *postfix = infix2postfix(infixExpression);
	for (int i = 0; postfix[i] != '='; i++) //take every char in postfix and calculate step by step to result
	{
		switch (postfix[i])
		{
			case '+':
				expr_value_pop(&stack, &val2);
				expr_value_pop(&stack, &val1);
				expr_value_push(&stack, val1 + val2);
				break;

			case '-':
				expr_value_pop(&stack, &val2);
				expr_value_pop(&stack, &val1);
				expr_value_push(&stack, val1 - val2);
				break;

			case '*':
				expr_value_pop(&stack, &val2);
				expr_value_pop(&stack, &val1);
				expr_value_push(&stack, val1 * val2);
				break;

			case '/':
				expr_value_pop(&stack, &val2);
				expr_value_pop(&stack, &val1);
				if (val2 == 0)
				{
					return false;
				}
				expr_value_push(&stack, val1 / val2);
				break;

			default:
				if (postfix[i] >= '0' && postfix[i] <= '9') //numbers save to stack
				{
					expr_value_push(&stack, postfix[i] - '0');
				}
				else
				{
					for (int j = 0; j < variableValueCount; j++) //for variables find its value and save it to stack
					{
						if (variableValues[j].name == postfix[i])
						{
							expr_value_push(&stack, variableValues[j].value);
							break;
						}
					}
				}
				break;
		}
	}
	expr_value_pop(&stack, value);
	Stack_Dispose(&stack); //cleaning memory
	free(postfix);
	return true;
}

/* Konec c204.c */
