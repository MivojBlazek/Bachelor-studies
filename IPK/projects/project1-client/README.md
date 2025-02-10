# Dokumentace k IPK projektu 1

## Popis programu

Tento program slouží jako klient pro komunikaci s ostatními klienty za pomoci serveru. Klient se musí přihlásit pomocí serverem určených  přihlašovacích údajů, a poté se klient může připojit do komunikačních kanálů. Klient je automaticky po přihlášení připojen do nějakého základního kanálu. Klienti následně můžou pomocí této aplikace komunikovat a případně ukončit spojení.

## Spouštění a operace v programu

### Spouštění programu

Po přeložení souboru pomocí příkazu `make` vznikne binární soubor `ipk24chat-client`. Tento binární soubor lze spustit s několika argumenty:

* `-t <tcp|udp>` je povinný argument, který určuje používaný protokol pro komunikaci,
* `-s <server_address>` je druhý povinný argument pro určení serveru, přes který se komunikuje (může být jak IP adresa, tak doménové jméno serveru),
* `-p <port>` vybírá port pro komunikaci. Když se nezadá, použije se "4567",
* `-d <timeout>` nastaví maximální dobu čekání v milisekundách na potvrzení přijetí zprávy od serveru během UDP komunikace. Základní hodnota je 250 milisekund,
* `-r <retransmissions>` je poslední argument, který určuje maximální počet pokusů o opětovné zaslání zprávy, pokud klient neobdrží potvrzení. V základu jsou povoleny 3 opětovné pokusy.

Pro výpis nápovědy k programu lze použít přepínač `-h`.

### Základní operace s programem

Po zapnutí program čeká na přihlášení. Uživatel má na výběr zase z několika příkazů, kde nápovědu k nim si vypíše pomocí `/help`.

Pro přihlášení slouží příkaz `/auth {username} {secret} {displayName}`. První dva parametry jsou přihlašovací údaje klienta a ten poslední značí jakousi přezdívku, pod kterou klient na serveru bude reprezentován. Tuto přezdívku si může kdykoliv změnit pomocí příkazu `/rename {displayName}`.

Jak již bylo zmíněno, klient se může připojit do určitých kanálů. To provede pomocí `/join {channelID}`.

Poslední možností je odeslání zprávy. Tato zpráva nemusí mít žádný formát a klient prostě napíše zprávu a odešle ji pomocí nového řádku.

## TCP komunikace

TCP komunikace je řešena v souboru tcpCommunication.c. Základními úkoly jsou vytvoření spojení mezi klientem a serverem, dále zpracovávání zpráv ze serveru a odesílání zpráv na server.

Většinu komunikace řeší funkce `TCPcommunication()`, která v případě potřeby zavolá ostatní funkce pro čtení nebo například vytvoření zprávy. Na základě dat z těchto funkcí se poté rozhodne, jakým způsobem bude komunikace pokračovat.

### Přijímání zpráv

Zprávy, které klient dostává, mají jasně daný formát. Toto zpracovávání řeší funkce `messageFromServerTCP()`. Funkce čte zprávu a snaží se ji rozdělit na části, které splňují syntaxi této zprávy. Pokud zpráva obsahuje nějaké specifické části, jako je například obsah normální zprávy, tuto část zkontroluje, zda podle specifikace splňuje velikost a povolené znaky. Následně funkce vrátí hodnotu reprezentující danou funkci nebo v případě nesrovnalostí vrací hodnotu reprezentující neznámou zprávu. Jestliže je zpráva v pořádku, klient vypíše informaci, která je ve zprávě obsažena.

Zprávy, které klient přijímá jsou:

* "ERR FROM {displayName} IS {messageContent}\r\n",
* "MSG FROM {displayName} IS {messageContent}\r\n",
* "REPLY OK IS {messageContent}\r\n",
* "REPLY NOK IS {messageContent}\r\n",
* "BYE\r\n".

Zpráva typu ERR značí chybu, která pravděpodobně nastala na serveru, MSG je normální zpráva. Pomocí BYE se server snaží ukončit spojení a REPLY je odpověď od serveru, kterou dostane klient vždy na zprávy AUTH a JOIN.

### Vytváření a odesílání zpráv

Pokaždé, když uživatel zadá příkaz, případně uživatel chce odeslat zprávu, musí se data od uživatele zpracovat a odeslat v odpovídajícím formátu na server. Zprávu vytváříme ve funkci `createMessageTCP()`, kde pomocí typu zprávy se vytvoří odpovídající zpráva. Tato zpráva je následně připravena k odeslání. O to se stará už rodičovská funkce `TCPcommunication()`.

## UDP komunikace

Komunikace pomocí UDP je uložena v souboru udpCommunication.c a je totožná s TCP s pár rozdíly. V UDP vždy potřebujeme potvrdit přijetí zprávy, takže využijeme hodnoty z programových argumentů `-d` a `-r`. Toto opětovné odesílání je implementováno pomocí do-while cyklů. 

### Přijímání zpráv

Zprávy jsou v UDP přijímány také podobně jako u TCP. Mají jiný formát, ale princip je podobný. Navíc se na každou zprávu odesílá potvrzení o přijetí a to i na neznámé nebo neočekávané zprávy.

### Vytváření a odesílání zpráv

Vytváření a odesílání zpráv je též podobné TCP až na formát. Jelikož formát UDP zprávy obsahuje znak konce řetězce '\0' jakožto oddělovač, musíme si společně se zprávou, až do jejího odeslání, pamatovat i její velikost.

## Hlavní část projektu

Hlavní část je v souboru ipk24chat-client.c, kde je nekonečný while cyklus, který se dá ukončit buď chybou, nebo pouze pomocí stisknutí Ctrl+C nebo Ctrl+D. Stisknutí této klávesové zkratky zapříčiní zavolání funkce `controlCorD()`, která má za úkol korektně ukončit komunikaci pomocí zprávy BYE a vyčistit použitou paměť.

Hlavní cyklus slouží pro opakované čekání na zprávy ze serveru nebo čekání na zadání zprávy klienta. Toto čekání je řešeno pomocí funkce select() s nulovou čekací dobou, kde vždy select() jenom zkontroluje, jestli není buď na komunikačním soketu se serverem, nebo na standardním vstupu nějaká informace a podle toho zpřístupní daný deskriptor souboru.

## Další a pomocné funkce

V souboru udpCommunication.c se nachází funkce `tryToExit()` na ukončení komunikace v případě přijetí nesprávné zprávy.

Další skupina funkcí je v souboru ipk24chat-client.c. Funkce `isAlphaNumericWord()`, `isPrintableWord()` a `isPrintableWithSpaces()` slouží pro kontrolu, zda uživatel zadal správný formát jednotlivých částí zprávy.

Funkce `join()`, `authenticate()` a `sendMessage()` řeší přípravu zprávy daného typu pro podrobnější zpracování funkcemi `createMessageUDP()` a `createMessageTCP()`. Následně zprostředkovávají komunikaci se serverem 

## Speciální struktury a výčty 

Mezi nejvýznamnější struktury programu patří `transmissionItems`, která uchovává informace o položkách potřebných pro komunikaci se serverem. Dále struktura `programArguments` slouží k uložení dat z argumentů programu a navíc tu je používaný soket a původní ID zprávy, na kterou očekáváme odpověď pomocí zprávy REPLY.

Program obsahuje také několik výčtů pro lepší čitelnost. Například `msgType` reprezentuje typ zprávy, která se bude vytvářet. Výčet `msgReturnType` se používá v zpracovávání zprávy od serveru.

## Testování

* Co bylo testováno:

    * Základní funkcionalita programu: Přihlášení klienta na server, změna komunikačního kanálu, odesílání a přijímání zpráv a korektní ukončení klienta.
    * Krajní případy: Několikanásobné neúspěšné přihlášení klienta, zaslání zprávy s nepovolenými znaky nebo nepovolenou délkou a například opětovné přihlašování i přes první úspěšný pokus.
    * Zotavování z chyb: Testování reakce programu na nevalidní zprávy serveru, vypořádání se s interními chybami programu, korektní ukončení spojení se serverem v chybových případech.

* Proč to bylo testováno:

    * Pro zajištění správné funkcionality programu a splnění všech požadavků.
    * Pro vylepšení jednotlivých specifických funkcionalit programu.

* Jak to bylo testováno:

    * Automatizovaným testováním, které se ukázalo jako vysoce náročné pro testovací prostředí.
    * Většina programu byla testována manuálně přímým psaním vstupu do programu.

* Testovací prostředí:

    * Operační systém: Ubuntu 22.04.4 LTS
    * Procesor: Intel Core i5 9300H
    * Paměť: 16GB DDR4
    * Software:
        * vscode
        * vestavěný terminál (netcat)
        * Discord
        * Wireshark

* Vstupy a výstupy testů:

    * Test 1:

        * Vstup:
        ```
        /auth username secret displayName   -- username and secret are valid credentials
        /join channel1                      -- channel1 is valid channelID
        Hello world!
        C-c                                 -- pressing Ctrl+C
        ```
        * Předpokládaný výstup:
        ```
        Success: Authentication successful.
        Server: displayName joined Default.
        Server: displayName joined channel1.
        Success: Channel channel1 successfully joined.
        """Hello world!"""                  -- message on server
        ```

    * Test 2:

        * Vstup:
        ```
        /join channel1
        C-c                                 -- pressing Ctrl+C
        ```
        * Předpokládaný výstup:
        ```
        ERR: User is not authenticated!
        ```

    * Test 3:

        * Vstup:
        ```
        /auth username secret displayName   -- username and secret are valid credentials
        /auth username secret displayName2
        C-c                                 -- pressing Ctrl+C
        ```
        * Předpokládaný výstup:
        ```
        Success: Authentication successful.
        Server: displayName joined Default.
        ERR: Already authenticated!
        ```

    * Test 4:

        * Vstup:
        ```
        /auth username secret displayName   -- username and secret are valid credentials
        Hello world!
        C-d                                 -- pressing Ctrl+D
        ```
        * Předpokládaný výstup:
        ```
        Success: Authentication successful.
        Server: displayName joined Default.
        """Hello world!"""                  -- message on server
        ```

    * Test 5:

        * Vstup:
        ```
        /auth username secret displayName   -- username and secret are valid credentials
        /join channel,2
        C-d                                 -- pressing Ctrl+D
        ```
        * Předpokládaný výstup:
        ```
        Success: Authentication successful.
        Server: displayName joined Default.
        ERR: Channel ID contains incompatible characters or it is too long!
        ```

    * Test 6:

        * Vstup:
        ```
        /autch username secret displayName
        /auth username secret displayName   -- username and secret are valid credentials
        C-d                                 -- pressing Ctrl+D
        ```
        * Předpokládaný výstup:
        ```
        Success: Authentication successful.
        Server: displayName joined Default.
        ```

    * Test 7:

        * Vstup:
        ```
        /auth usernamE secret displayName   -- secret is valid credential
        /auth username secret displayName   -- username and secret are valid credentials
        C-d                                 -- pressing Ctrl+D
        ```
        * Předpokládaný výstup:
        ```
        Failure: Authentication failed - No such user identity usernamE with provided secret found.
        Success: Authentication successful.
        Server: displayName joined Default.
        ```

## Použité zdroje

Zadání projektu: https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master/Project%201

Funkce a knihovny jazyka C: https://devdocs.io/c/

Funkce getaddrinfo(): https://beej.us/guide/bgnet/html/#getaddrinfoprepare-to-launch

Vscode podpora POSIXU: https://github.com/Microsoft/vscode-cpptools/issues/2025
