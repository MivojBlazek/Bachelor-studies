# Zadání:

## Jednoduchý 2D simulátor mobilních robotů.

V ohraničeném prostředí obdélníkového půdorysu je situován libovolný počet robotů.  Robot má tělo kruhového půdorysu a je schopen pohybu vpřed danou rychlostí, otočení o daný úhel a detekce kolizí. V prostředí se mohou vyskytovat statické překážky. Překážky jsou tvořeny libovolným počtem čtverců dané velikosti na zvolených souřadnicích. Počáteční rozmístění překážek a robotů (včetně jejich orientace a dalších parametrů) lze stanovit interaktivně v GUI, lze ho uložit do souboru v čitelné a ručně editovatelné textové podobě  a načíst. 

Podle způsobu řízení lze rozlišit 2 typy robotů: 

1. Autonomní robot se ve volném prostředí pohybuje přímočaře a vyhýbá se kolizím tak, že při detekci překážky v dané vzdálenosti (parametr robota) se otočí o daný úhel (parametr robota) daným směrem (parametr robota) a pokračuje stejným způsobem dál.

2. Dálkově ovládaný robot přechází podle pokynů operátora mezi stavy žádný pohyb, pohyb vpřed, otáčení proti směru hodinových ručiček, otáčení po směru h.r. Pokud detekuje překážku, zastaví svůj pohyb vpřed.

GUI umožňuje ovládání simulátoru a vizualizuje simulaci. Zobrazuje mapu prostředí s překážkami a roboty a obsahuje ovládací prvky pro dálkové ovládání vybraného robota. Simulaci lze spustit z výchozího stavu, pozastavit, nechat pokračovat. Simulace pracuje se zjednodušenou fyzikou, pracující s diskrétním časem, spojitým 2D prostorem a rovnoměrným přímočarým pohybem danou rychlostí a rovnoměrnou rotací danou úhlovou rychlostí (nebere se v úvahu hmotnost, setrvačnost, zrychlení apod., pouze vzdálenost, rychlost, úhlová rychlost a čas). Simulace i vizualizace probíhá ve vhodně zvolených časových krocích. Kroky by měly být dostatečně jemné (vzhledem k rychlosti pohybu), aby robot stihl detekovat překážku dříve, než dojde ke kolizi a také aby vizualizace probíhala zdánlivě plynule.



## Pokyny k realizaci projektu:

* Zadání definuje podstatné vlastnosti aplikace, které musí být splněny. Předpokládá se, že detaily řešení si doplní řešitelské týmy.

* Při návrhu postupujte tak, aby výsledný program byl dobře použitelný. Hodnotit se bude nejen to, zda splňuje výše uvedené požadavky, ale také JAK je splňuje.

* Návrh musí oddělit vlastní model aplikace od GUI (navrhněte vhodné rozhraní).

* Návrh programu zdokumentujte ve zdrojovém kódu ve formátu pro program "doxygen".

* Používejte vhodné návrhové vzory (Design Patterns) - viz literatura.

* Pro vytváření menu, dialogů a dalších komponent grafického uživatelského rozhraní (GUI) použijte toolkit Qt minimálně ve verzi 5.5 (nainstalováno v učebnách a na serveru "merlin" v adresáři /usr/local/share/Qt-5.5.1/). Pokud pro vývoj použijete verzi novější/starší než je na učebnách, předpokládáme, že zajistíte zpětnou/dopřednou kompatibilitu vašich programů. (V případě problémů, počítejte s nutností předvedení překladu/sestavení/běhu aplikace na vašem notebooku. Toto se nedoporučuje.)

* Protože toolkity jsou přenositelné na více platforem, měly by vaše programy fungovat minimálně na Linuxu i na Windows. (Nejen Java umožňuje psaní přenositelných aplikací.) Je jedno na které platformě proběhne vývoj aplikací, jen dbejte na pravidla psaní přenositelných programů. (Testovat se bude na Linuxu = server merlin, ale Makefile nesmí být závislý na konkrétním adresáři s lokální instalací toolkitu.)

* Na vytvoření GUI dialogů a menu můžete použít interaktivní editory/generátory.

* Použijte C++17. (Boost raději nepoužívejte. Pokud použijete některé knihovny z Boost, tak použité soubory přibalte a zkontrolujte si, zda se to vše vejde do max. velikosti odevzdaného archivu. Existuje nástroj BCP pro výběr podmnožiny Boost.).

* Programy budou vytvořeny zadáním příkazu "make" v adresáři s rozbalenými zdrojovými kódy projektu (Pozor - bude se automaticky překládat). Musíte vytvořit soubor Makefile, který nesmí obsahovat absolutní cesty k souborům.

* Výsledný program vhodně pojmenujte. Pro testovací účely půjde vše spustit použitím "make run".

* Součástí projektu bude programová dokumentace vytvořená programem Doxygen (konfigurační soubor nastavte na HTML výstup se zahrnutím zdrojových textů: SOURCE_BROWSER = YES). Dokumentace se vygeneruje automaticky (= nezabalovat do archivu) při zadání "make doxygen" a bude uložena v podadresáři doc.

* Součástí projektu bude také konceptuální návrh v pdf, obsahující minimálně diagram tříd bez zbytečných implementačních detailů, případně další UML diagramy dokumentující návrh. Očekává se, že implementace bude v souladu s konceptuálním návrhem.



## Poznámky k odevzdávání

* Termín odevzdání je uveden v IS.

* Řešení zabalené ve formátu .zip (nebo .tar.gz) odevzdá pouze vedoucí týmu do IS a to s dostatečným předstihem před uzávěrkou. (Nikdo nebude psát, že neodevzdal, protože *** a posílat to poštou.) Jméno odevzdaného souboru musí mít tvar: 

        xname01-xname02.zip

* xname01 je jméno vedoucího (toho, kdo projekt odevzdal), xname02 je druhý člen týmu.

* POZOR:  Velikost archivu musí odpovídat tomu, co dovolí IS. Proto si raději včas ověřte odevzdtelnost, ať nedojde k problému na poslední chvíli.

* Odevzdaný archiv obsahuje minimálně tuto adresářovou strukturu:

    - ./src/* - zdrojové texty

    - ./examples/* - příklady (pro testování)

    - ./doc/ - dokumentace (bude později generován: "make doxygen")

    - ./README.txt - základní přehled o co jde, autoři, poznámky, ...

    - ./Makefile - zavolá rekurzivně make na src/Makefile, případně na generování dokumentace atd.

* Neodevzdávejte nic, co lze vytvořit pomocí "make" (spustitelné programy ani dokumentaci, kterou lze vygenerovat použitím "make doxygen").

> Poznámka:
>
> Příkaz "make clean" všechny produkty překladu smaže, "make pack" vytvoří archiv pro odevzdání - zkontrolujte, zda obsahuje vše potřebné rozbalením v prázdném adresáři a překladem/spuštěním.

* Všechny zdrojové soubory musí obsahovat na začátku komentář se jmény autorů daného modulu, popisem obsahu modulu, atd. Z komentářů by také mělo být patrné, které části byly napsány ručně a které vygenerovány automaticky.

* Pokud některý modul vychází z převzatého kódu (např. z nějakého tutorialu nebo veřejně dostupného opensource projektu), musí být toto explicitně specifikováno v záhlaví modulu a také zmíněno v README.

* V dokumentaci (README) musí být zřetelně specifikováno, jaká funkcionalita byla implementována kompletně, jaká s nějakými omezeními a jaká vůbec (aby se hodnotitel zbytečně nesnažil zprovoznit něco, o čem je dopředu známo, že je nefunkční). Nesrovnalost mezi dokumentací a realitou bude předmětem bodové penalizace.



## Poznámky k hodnocení

* Projekt je hodnocen na stupnici 0-100 procent/bodů.

* Po ohodnocení může dojít k přepočtu bodů na jednotlivé členy týmu podle návrhu vedoucího týmu, který je zakódován do jména odevzdaného souboru (formát jména pak je:    

      xname01-xname02-AA-BB.zip 

* Součet AA+BB=100, POZOR: pro rovnoměrné rozdělení AA=BB=50% není třeba uvádět "-50-50" a je to preferovaná možnost).

* Hodnocení zahrnuje kvalitu OO návrhu, kvalitu implementace v C++, dodržování stylu psaní programu (odsazování, kvalita komentářů, vhodné identifikátory) a především funkčnost programu. Pokud budou ve zdrojových textech chybět podstatné komentáře (zvláště jména autorů), bude projekt hodnocen 0b.

* POZOR: 

    - Pokud nepůjde přeložit příkazem make = 0b nebo předvedete na svém **notebooku a vysvětlíte kde byl problém,

    - pokud nebudou komentáře se jmény autorů = 0b.

    - Zcela nevhodně formátovaný a nečitelný kód = 0b.

    - Plagiáty atd. = 0b + návštěva u disciplinární komise