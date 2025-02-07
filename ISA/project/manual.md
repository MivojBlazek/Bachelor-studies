### Obsah dokumentace

* Uvedení do problématiky
* Návrh aplikace
* Popis implementace
* Základní informace o programu
* Návod na použití
* Popis testování aplikace a výsledky testů

### Struktura dokumentace

* Jako technická zpráva
    * Titulní strana
    * Obsah
    * Logické strukturování textu včetně číslování kapitol
    * Přehled nastudovaných informací z literatury
    * Popis zajímavějších pasáží implementace
    * Použití vytvořených programů
    * Literatura
* Šablona bakalářské práce



# Obsah

## Uvedení do problématiky

Hlavním cílem programu je zachytávání a výpis DNS zpráv. DNS zprávy slouží pro překlad doménových jmen, která jsou pro člověka lehce zapamatovatelná, na IP adresy, se kterými zase lépe pracují počítače.

Při monitorování této komunikace můžeme nalézt záznamy, které bychom neočekávali nebo obráceně očekáváme záznam, který třeba ani nepřijde. Díky tomu lze odhalit škodlivou aktivitu, nebo třeba problémy s připojením.

## Návrh aplikace

Původní návrh aplikace odpovídá vlastně funkci main(), a to je zpracování vstupních parametrů programu a následné naslouchání a zachytávání DNS záznámů v protokolu UDP.

DnsMonitor zpracuje parametry programu a předá řízení DnsCapturer, který následně na rozhraní začne naslouchat nebo zpracuje soubor formátu PCAP. DnsCapturer vytvoří pro každý záznam instanci třídy Packet a ta si pomocí DnsHeader pomůže pro zpracování informací z DNS hlaviček. Instance třídy Packet pak mají možnost vypsat data o daném zachyceném záznamu.

## Popis implementace

Jelikož návrh aplikace byl objektově orientovaný, bylo vhodné zvolit i objektově orientovaný jazyk, tudíž je program napsaný v jazyce C++.

Počáteční funkce programu main() vytvoří instanci třídy DnsMonitor, která zpracuje vstupní parametry a vytvoří instanci třícy PacketCapturer. PacketCapturer otevře rozhraní nebo PCAP soubor pro naslouchání, nastaví filtr pouze na DNS záznamy na portu 53 a pouze na protokol UDP. Tyto záznamy poté zachytává a každý paket ukládá do nové instance třídy Packet.

Packet se stará o zpracování dat do čitelné struktury, kde si uloží zdrojovou a cílovou IP adresu a port. Při vypsání informací z paketů si instance třídy Packet pomůže pomocí DnsHeader. DnsHeader zpracovává informace v DNS záznamů a případně jeho doplňkových záznamech v sekcích Questions, Answer, Authority a Additional.

Největších oříškem je zpracování doménových jmen v sekcích Answer, Authority a Additional, protože z důvodu ušetření velikosti paketů se zde může místo celého doménového jména nacházet jen část a byte mající první 2 bity v 1 (0xC0 až 0xFF). Toto značí skok na jiný byte v DNS záznamu a těchto skoků může být několik před přečtením celého doménového jména. Řešeno je to rekurzivně volaním metody pro dokódování doménového jména, kde se pak jednotlivé části pospojují.

## Základní informace o programu

Program zachycuje veškerou DNS komunikaci na daném rozhraní přes protokol UDP, nebo může program zpracovat tuto komunikaci v souboru ve formátu PCAP.

Program vypisuje informace z DNS zpráv buď v zjednodušeném, nebo v kompletním výpisu na standardní výstup. V případě potřeby může vypisovat nalezená doménová jména do zvoleného souboru a stejným způsobem může vypisovat překlady doménových jmen na IP adresy do dalšího zvoleného souboru.

V kompletním výpisu DNS zpráv nalezneme pouze informace o A, AAAA, NS, MX, SOA, CNAME a SRV typech záznamů. Ostatní typy záznamů program ignoruje a nevypisuje.

## Návod na použití

V archivu je přiložen soubor Makefile, tudíž pro přeložení programu stačí mít nainstalovaný balíček make a použít stejnojmenný příkaz make.
Po přeložení programu vznikne binární soubor dns-monitor. Tento binární soubor lze spustit s několika argumenty:

* -i <interface> je argument udávající rozhraní, na kterém bude program poslouchat,
* -p <pcapfile> určuje soubor, který program zpracuje a pokusí se v něm nalézt DNS komunikaci,
* -v zapíná kompletní výpis detailů o DNS zprávách,
* -d <domainsfile> slouží pro výběr souboru, do kterého se případně uloží nalezená doménová jména,
* -t <translationsfile> je poslední argument, který určuje kam se mají uložit případné překlady adres, které se během komunikace naleznou.

Argumenty -v, -d <domainsfile> a -t <translationsfile> jsou volitelné, ale program vyžaduje použití -i <interface> pro naslouchání na daném rozhraní, případně -p <pcapfile> pro zpracování souboru, avšak program nesmí dostat oba zároveň. Může buď naslouchat na rozhraní, nebo zpracovat soubor.

V případě potřeby, může uživatel ukončit naslouchání na rozhraní pomocí zaslání signálu SIGINT skrz klávesovou zkratku Ctrl+C.

## Popis testování aplikace



### Výsledky testů
