# Implementační dokumentace k 2. úloze do IPP 2023/2024

Jméno a příjmení: Michal Blažek

Login: xblaze38

## Popis programu

Tento program slouží jako interpret jazyka XML v odpovídajícím formátu, který je kompatibilní s výstupem programu `parse.py`. Dá se tedy říct, že oba programy zvládnou spolu interpretovat kód jazyka IPPcode24. Program je schopný detekovat sémantické chyby vstupní XML reprezentace. Jelikož navazuje na překladač jazyka IPPcode24, který již nalezl lexikální a syntaktické chyby, tak tento interpret už nemusí nějak moc testovat některé případy.

## Popis jednotlivých souborů projektu

Projekt je implementován pomocí objektově orientovaného programování v jazyce PHP, konkrétně PHP ve verzi 8.3. Projekt se skládá z několika tříd, kde každá je implementována ve svém vlastním souboru, který je pojmenován stejně jako zmíněné třídy.

### Soubor Interpreter.php

Tento soubor obsahuje třídu `Interpreter` a její jedinou metodu `execute()`. Tato metoda je volána na začátku programu a slouží pro kontrolu argumentů programu, základní kontrolu značek v XML souboru a rozdělení XML souboru na značky, kde zpracuje veškeré značky a předá řízení metodě `executeProgram()` ze třídy `ProgramTag`.

### Soubor ProgramTag.php

Slouží pro zpracování značky `program` v XML. Má třídní atribut pole instrukcí `$instuctions` pro uchovávání instrukcí v rámci dané značky `program` a další pole `$labels`, kde si uchováváme informace o návěštích.

Již byla zmíněna metoda `executeProgram()`, která právě na začátku připravuje prostředí pro interpretaci. Vytvoří se instance třídy `Frame` reprezentující rámce. Dále se vytvoří 2 instance třídy `Stack`. Tato metoda také pomocí další metody této třídu `findLabels()` naplní třídní atribut pole návěští pro budoucí skoky v programu na určitá návěští. Nakonec metoda projde všechny instrukce a každou zpracovává pomocí `executeInstr()` metody ze třídy `InstructionTag`.

Další zajímavá metoda je `sortInstructions()`, která slouží pro vzestupné srovnání všech instrukcí podle jejich atributu `order`.

### Soubor `InstructionTag.php`

Nejrozsáhlejší třída `InstructionTag` uchovává atributy značky `instruction` a její argumenty. Navíc provádí kontrolu v počtu a pořadí těchto argumentů.

Hlavní metoda `executeInstr()`, která zpracovává jednotlivé instrukce se skládá vlastně z velkého přepínače, který pouze vybírá podle typu instrukce tu danou instrukci pro zpracování. V případě skoku v programu, je vrácena hodnota návěští a o tento skok se stará metoda `executeProgram()` ve třídě `ProgramTag`.

### Soubor `ArgumentTag.php`

V tomto souboru je stejnojmenná třída `ArgumentTag` sloužící pro uchovávání informací o značkách, které reprezentují argumenty instrukce. Tyto značky jsou v programu jako `arg1`, `arg2` a `arg3`. Typicky tato třída pouze uchovává informace, takže lze pomocí konstruktoru nastavit třídní vlastnosti `$type` a `$value`, které reprezentují stejnojmenné atributy v XML reprezentaci a pomocí metod `getType()` a `getValue()` lze tyto hodnoty získat.

### Soubor `Frame.php`

Obsahuje třídu `Frame`, která udržuje v programu informace o všech aktivních rámcích. Lze pomocí metod `createTempFrame()`, `createLocalFrame()` a `popLocalFrame()` vytvořit dočasné rámce, ze kterých můžeme udělat lokální rámce. Tyto rámce se vkládají na zásobník rámců `$frameStack` a ten je implementován pomocí pole, kde na nulté pozici obsahuje globální rámec programu.

Samozřejmě potřebujeme do těchto rámců vkládat proměnné. Pomocí metody `addVariable()` tohoto docílíme. Pro ošetření problémů s vkládáním proměnné do rámce, ve kterém tato proměnná už existuje je zde metoda `getVarIfExists()`, která po zavolání zkontroluje daný rámec a pokusí se najít určitou proměnnou. Pokud není nalezena vrací NULL, jinak vrátí nalezenou proměnnou.

### Soubor `Variable.php`

Zde nalezneme informace o proměnných. Každá si uchovává svůj datový typ, jméno a hodnotu. S proměnnými pracujeme pomocí metod `setValue()` v případě potřeby na nastavení nové hodnoty a nebo pokud potřebujeme z proměnné získat její hodnotu, typ, nebo vůbec její jméno, použijeme příslušné `get...()` metody. Jelikož IPPcode24, ze kterého vychází zdrojové XML, používá dynamické typování proměnných, datový typ vždy kontroluje hodnotu proměnné a až podle ní usoudí, jaký typ tato proměnná má.

### Soubor `Stack.php`

Tento soubor má třídu `Stack`, ve které je třídní atribut pole položek zásobníku. Samozřejmě na zásobník lze dát položku pomocí metody `stackPush()` a získat z něj položku pomocí `stackPop()`.

