# Dokumentace k IPK projektu 2

## Popis programu

Program slouží jako server pro zprovoznění komunikace mezi klienty, kde klient používá aplikaci z 1. projektu. Server zprostředkovává jakýsi prostor s komunikačními kanály, ve kterých mohou klienti komunikovat pomocí zasílání zpráv serveru, který je přeposílá ostatním klientům v daném kanálu.

## Spouštění a operace v programu

### Spouštění programu

Po přeložení souboru pomocí příkazu `make` vznikne binární soubor `ipk24chat-server`. Tento binární soubor lze spustit s několika argumenty:

* `-l <listening_address>` je argument pro určení IP adresy serveru, na které bude server navazovat spojení a při nevyužití tohoto argumentu se použije adresa 0.0.0.0,
* `-p <listening_port>`je port pro komunikaci s klienty. Když se nezadá, použije se "4567",
* `-d <timeout>` nastaví maximální dobu čekání v milisekundách na potvrzení přijetí zprávy od klientů během UDP komunikace. Základní hodnota je 250 milisekund,
* `-r <retransmissions>` je poslední argument, který určuje maximální počet pokusů o opětovné zaslání zprávy, pokud klient neobdrží potvrzení. V základu jsou povoleny 3 opětovné pokusy.

Pro výpis nápovědy k programu lze použít přepínač `-h`.

### Základní operace s programem

Po spuštění aplikace server čeká na připojení klientů a stejně tak po ukončení spojení se všemi klienty server zůstává stále aktivní. Pro vynucení ukončení serveru lze použít zaslání signálů `C-c` nebo `C-d` (implementaci najdeme ve funkci `controlCorD()`). Veškeré ostatní operace provádí server automaticky na základě zpráv od klientů.

## TCP komunikace

TCP komunikace s klientem byla původně řešena pomocí procesů, avšak toto se ukázalo jako nevhodné použití pro komunikaci mezi klienty. I přes sdílení čísla deskriptoru každého klienta mezi všemi klienty, nemohli nejdéle připojení klienti odesílat zprávy novým klientům. Problém byl samozřejmě v tom, že tito klienti neměli otevřený tento souborový deskriptor, a tudíž nemohli odeslat zprávu těmto klientům, zatímco noví klienti vždy zdědili tabulku souborových deskriptorů od hlavního procesu, který sloužil k navazování spojení s klienty a vytvoření nového procesu pro nového klienta, a měli informaci o již aktivních klientech.

Po několika neúspěšných pokusech o odeslání zprávy od klienta dalším klientům, byla vyzkoušena vlákna. Implementace pomocí vláken se zdá mnohem funkčnější, jelikož vlákna mezi sebou sdílejí tyto prostředky, jako jsou souborové deskriptory.

### Přijímání zpráv

Server přijímá zprávy vlastně stejně jako klient, jen očekává jiné zprávy. Implementaci přijímání zpráv a jejich zpracování řeší funkce `messageFromClientTCP()`. Pro přihlášení klient používá autentizační zprávu `AUTH` a pro připojení do jiného komunikačního kanálu zase zprávu pro toto přepojení `JOIN`. Všechny ostatní zprávy (normální zpráva `MSG`, chybová zpráva `ERR` a zpráva ukončující spojení `BYE`) jsou přijímány serverem naprosto stejně jako v rámci klienta.

Formát jednotlivých zpráv pro TCP komunikaci:

* "ERR FROM {displayName} IS {messageContent}\r\n",
* "MSG FROM {displayName} IS {messageContent}\r\n",
* "AUTH {username} AS {displayName} USING {secret}\r\n",
* "JOIN {channelID} AS {displayName}\r\n",
* "BYE\r\n".

Pokud nějaká zpráva je poškozená nebo jednotlivé části nesplňují požadavky na délku nebo na povolené znaky, server korektně ukončuje spojení s klientem s odesláním chybové zprávy a zprávy `BYE`.

## UDP komunikace

UDP komunikace s klienty je v základu úplně odlišné od TCP komunikace. Narozdíl od vytváření vláken s novými klienty v TCP je v UDP vytvořeno vlákno jedno při prvním připojení klienta. Toto vlákno se následně stará o veškerou komunikaci v UDP od všech klientů připojených pomocí tohoto protokolu. To znamená, že server musí vždy zkontrolovat, zda určitý klient je přihlášen (nebo není v případě, že server dostane autentizační zprávu) a teprve až potom může zpracovávat danou zprávu.

Další odlišností od protokolu TCP je zasílání potvrzovacích zpráv. Pokud tato zpráva nepřijde do požadovaného času určeném argumentem `-d`, je zpráva opětovně odeslána, a to až do maximálního počtu opětovného odeslání z argumentu `-r`.

### Přijímání zpráv

Řešení implementace zpracovávání těchto práv můžeme najít ve funkci `messageFromClientUDP()`. Funguje to na stejném principu jako v případě TCP, jenom zprávy mají odlišný formát. Navíc se na každou zprávu odesílá potvrzovací zpráva o přijetí.

## Vytváření zpráv

Vytváření zprávy pro odeslání určitému klientovi je ve funkcích `createMessageTCP()` a `createMessageUDP()`, kde každá z těchto funkcí se stará o daný transportní protokol.

## Odesílání zpráv a přeposílání zpráv od klientů

K odesílání zpráv ze strany serveru dochází pouze u zpráv `CONFIRM`, `REPLY` a v případě chybových zpráv a ukončování spojení. První zmíněnou můžeme najít pouze u protokolu UDP, kde se využívá pro potvrzení přijetí jakékoliv zprávy od klienta (stejně tak klient potvrzuje zprávy i serveru). Druhá zmíněná slouží jako odpověď na zprávy typu `AUTH` a `JOIN` určitému klientovi.

Během připojení a odpojení klienta do komunikačního kanálu a z komunikačního kanálu je zaslána normální informační zpráva všech klientům v daném kanálu. Stejně při přijetí zprávy od klienta, je tato zpráva přeposlána všem klientům v daném komunikačním kanálu.

Jelikož klient, který je připojený pomocí TCP protokolu, může odeslat zprávu serveru, která má být přeposlána ostatním klientům a ti mohou být ale připojeni přes UDP protokol, musí se tato zpráva převést na formát UDP zprávy. To může vzniknout ale i obráceně, a proto máme v kódu funkce `TCPtoUDP()` a `UDPtoTCP()` určené pro tento převod.

## Soubory projektu

Projekt se skládá ze souborů `tcpCommunication.c`, `udpCommunication.c` `ipk24chat-server.c` a `users.c`. První 2 soubory byly již popsány výše.

Soubor `users.c` obsahuje práci s polem klientů, které je ve sdílené paměti programu. Funkce `authenticationSuccessful()` kontroluje, zda klient zadal správné přihlašovací údaje. Dále zde najdeme funkce pro přidávání klienta do pole aktivních klientů `addUserToActive()` a jeho odstranění `removeUser()`, nalezení klienta, pokud v tomto poli už nějaký je, provedeme pomocí funkce `findUser()` a případně pro nalezení všech klientů ve stejném komunikačním kanálu pomocí funkce `findUsersInSameChannel()`.

Poslední soubor `ipk24chat-server.c` obsahuje hlavní tělo projektu. Zde se vytváří sdílená paměť, zpracovávají se argumenty programu a vytváří se nová vlákna v případě nového TCP klienta a jedno nové vlákno v případě UDP klienta. Tato vlákna přechází do funkcí `clientHandleTCP()` a `clientHandlerUDP()`. V UDP reprezentaci se čeká na zprávy a podle přijetí zprávy se zavolá funkce `authUDP()`, `joinUDP()` nebo `msgUDP()` (ostatní zprávy jsou zpracovávány přímo). V TCP se postupuje podle konečného automatu postupně přes stavy, které odpovídají názvům funkcí, které je zpracovávají (`acceptStateTCP()`, `authStateTCP()`, `openStateTCP()` a `errorStateTCP()`).

## Další a pomocné funkce

Mezi pomocné funkce, sloužící pro kontrolu povolených znaků v řetězci (například v {secret} v `AUTH` zprávě), můžeme zařadit funkce `isAlphaNumericWord()`, `isPrintableWord()` a `isPrintableWithSpaces()`.

Další pomocnou funkcí je například `tryToExitUDP()`, která se snaží v případě chyby korektně ukončit spojení s daným klientem, nebo ho pomocí chybové zprávy `ERR` informuje.

## Speciální struktury a výčty 

Mezi nejvýznamnější struktury programu patří `transmissionItems`, která uchovává informace o položkách potřebných pro komunikaci se serverem. Dále struktura `programArguments` slouží k uložení dat z argumentů programu a navíc tu je používaný soket a ID zpráv, které postupně zvyšujeme při odesílání zpráv klientům. V této implementaci je toto ID společné pro všechny klienty, takže každému klientu nemusí nutně přicházet ID v pořadí s inkrementováním o 1. Struktura `message` obsahuje typ zprávy pro snadnou identifikaci v průběhu programu, její obsah a velikost. Poslední struktura je `user`, kde si uchováváme u každého klienta jeho údaje, aktuální komunikační kanál, jeho soket, adresu, využívaný transportní protokol a v neposlední řadě jakési ID, které je vlastně ve formátu "{IP_adresa_klienta}:{port_klienta}". Tímto bychom měli dosáhnout jedinečnosti pro vyhledávání klientů v poli aktivních klientů.

Program obsahuje také několik výčtů pro lepší čitelnost. Například `msgType` reprezentuje typ zprávy, která se bude vytvářet. Výčet `msgReturnType` se používá během zpracovávání zprávy od klienta.

## Testování

* Co bylo testováno:

    * Základní funkcionalita programu: Přihlášení klientů na server, změna komunikačního kanálu klienta, odesílání a přijímání zpráv a korektní ukončení klientů.
    * Obtížnější funkcionalita programu: Přijetí zprávy od klienta a její následné přeposlání ostatním klientům ve stejném komunikačním kanálu. Odeslání zprávy všem účastníkům daného kanálu s informací o připojení / odpojení klienta do / z komunikačního kanálu.
    * Krajní případy: Několikanásobné neúspěšné přihlášení klienta, přijetí zprávy od klienta s nepovolenými znaky nebo nepovolenou délkou.
    * Zotavování z chyb: Testování reakce programu na nevalidní zprávy od klientů, vypořádání se s interními chybami programu, korektní ukončení spojení s daným klientem v chybových případech.

* Proč to bylo testováno:

    * Pro dosažení maximální možné funkcionality a ošetření chybových případů.

* Jak to bylo testováno:

    * Většina programu byla testována manuálně přímým psaním vstupu do programů klientů.

* Testovací prostředí:

    * Operační systém: Ubuntu 22.04.4 LTS
    * Procesor: Intel Core i5 9300H
    * Paměť: 16GB DDR4
    * Software:
        * vscode
        * vestavěný terminál (netcat)
        * ipk24chat-client aplikace z 1. projektu
        * Wireshark

* Vstupy a výstupy 

    * Test 1:

        * Vstup:
        ```
        ClientTCP1:   /auth admin admin TCP1
        ClientTCP2:   /auth admin admin TCP2
        ClientUDP1:   /auth admin admin UDP1
        ClientUDP2:   /auth admin admin UDP2
        ClientUDP2:   hello
        ClientTCP1:   hi
        ClientUDP1:   hey
        ClientTCP2:   hello
        ClientUDP1:   ^C
        ClientUDP2:   ^C
        ClientTCP2:   ^C
        ClientTCP1:   ^C
        ```
        * Předpokládaný výstup:
        ```
        RECV 127.0.0.1:60312 | AUTH
        SENT 127.0.0.1:60312 | REPLY
        RECV 127.0.0.1:46156 | AUTH
        SENT 127.0.0.1:46156 | REPLY
        RECV 127.0.0.1:53526 | AUTH
        SENT 127.0.0.1:53526 | CONFIRM
        SENT 127.0.0.1:53526 | REPLY
        RECV 127.0.0.1:53526 | CONFIRM
        RECV 127.0.0.1:45401 | AUTH
        SENT 127.0.0.1:45401 | CONFIRM
        SENT 127.0.0.1:45401 | REPLY
        RECV 127.0.0.1:45401 | CONFIRM
        RECV 127.0.0.1:45401 | MSG
        SENT 127.0.0.1:45401 | CONFIRM
        RECV 127.0.0.1:60312 | MSG
        RECV 127.0.0.1:53526 | MSG
        SENT 127.0.0.1:53526 | CONFIRM
        RECV 127.0.0.1:46156 | MSG
        RECV 127.0.0.1:53526 | BYE
        SENT 127.0.0.1:53526 | CONFIRM
        RECV 127.0.0.1:45401 | BYE
        SENT 127.0.0.1:45401 | CONFIRM
        RECV 127.0.0.1:46156 | BYE
        RECV 127.0.0.1:60312 | BYE
        ```

    * Test 2:

        * Vstup:
        ```
        ClientTCP1:   /auth admin admin TCP1
        ClientUDP1:   /auth admin admin UDP1
        ClientUDP1:   hey
        ClientTCP1:   /join channel1
        ClientUDP1:   hey
        ClientTCP1:   hi
        ClientUDP1:   /join channel1
        ClientUDP1:   hey
        ClientTCP1:   ^C
        ClientUDP1:   ^C
        ```
        * Předpokládaný výstup:
        ```
        RECV 127.0.0.1:35876 | AUTH
        SENT 127.0.0.1:35876 | REPLY
        RECV 127.0.0.1:55211 | AUTH
        SENT 127.0.0.1:55211 | CONFIRM
        SENT 127.0.0.1:55211 | REPLY
        RECV 127.0.0.1:55211 | CONFIRM
        RECV 127.0.0.1:55211 | MSG
        SENT 127.0.0.1:55211 | CONFIRM
        RECV 127.0.0.1:35876 | JOIN
        SENT 127.0.0.1:35876 | REPLY
        RECV 127.0.0.1:55211 | MSG
        SENT 127.0.0.1:55211 | CONFIRM
        RECV 127.0.0.1:35876 | MSG
        RECV 127.0.0.1:55211 | JOIN
        SENT 127.0.0.1:55211 | CONFIRM
        SENT 127.0.0.1:55211 | REPLY
        RECV 127.0.0.1:55211 | CONFIRM
        RECV 127.0.0.1:55211 | MSG
        SENT 127.0.0.1:55211 | CONFIRM
        RECV 127.0.0.1:35876 | BYE
        RECV 127.0.0.1:55211 | BYE
        SENT 127.0.0.1:55211 | CONFIRM
        ```

    * Test 3:

        * Vstup:
        ```
        ClientTCP1:   /auth admin admin TCP1
        ClientTCP1:   hi
        ClientTCP1:   ^C
        ClientTCP1:   /auth admin admin TCP1
        ClientTCP1:   hi
        ClientTCP1:   ^C
        ```
        * Předpokládaný výstup:
        ```
        RECV 127.0.0.1:44018 | AUTH
        SENT 127.0.0.1:44018 | REPLY
        RECV 127.0.0.1:44018 | MSG
        RECV 127.0.0.1:44018 | BYE
        RECV 127.0.0.1:53858 | AUTH
        SENT 127.0.0.1:53858 | REPLY
        RECV 127.0.0.1:53858 | MSG
        RECV 127.0.0.1:53858 | BYE
        ```

    * Test 4:

        * Vstup:
        ```
        ClientTCP1:   /auth admin admin TCP1
        ClientUDP1:   /auth admin admin UDP1
        ClientUDP2:   /auth admin admin UDP3
        ClientUDP2:   hi
        ClientUDP2:   /rename UDP2
        ClientUDP2:   hello
        ClientUDP2:   ^C
        ClientUDP1:   ^C
        ClientTCP1:   ^C
        ```
        * Předpokládaný výstup:
        ```
        RECV 127.0.0.1:59620 | AUTH
        SENT 127.0.0.1:59620 | REPLY
        RECV 127.0.0.1:48348 | AUTH
        SENT 127.0.0.1:48348 | CONFIRM
        SENT 127.0.0.1:48348 | REPLY
        RECV 127.0.0.1:48348 | CONFIRM
        RECV 127.0.0.1:35432 | AUTH
        SENT 127.0.0.1:35432 | CONFIRM
        SENT 127.0.0.1:35432 | REPLY
        RECV 127.0.0.1:35432 | CONFIRM
        RECV 127.0.0.1:35432 | MSG
        SENT 127.0.0.1:35432 | CONFIRM
        RECV 127.0.0.1:35432 | MSG
        SENT 127.0.0.1:35432 | CONFIRM
        RECV 127.0.0.1:35432 | BYE
        SENT 127.0.0.1:35432 | CONFIRM
        RECV 127.0.0.1:48348 | BYE
        SENT 127.0.0.1:48348 | CONFIRM
        RECV 127.0.0.1:59620 | BYE
        ```

## Použité zdroje

Zadání projektu: https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master/Project%202/iota

Dokumentace k 1. projektu předmětu IPK: https://git.fit.vutbr.cz/xblaze38/IPK24-Project1/src/branch/main/README.md

Sdílení souborového deskriptoru mezi procesy: https://forums.openqnx.com/t/topic/5098

Použití vláken namísto procesů: https://www.unix.com/programming/162202-how-can-i-share-socket-between-childs.html

Stevens, W. R., & Rago, S. A. (2013). Advanced Programming in the UNIX® Environment (3rd ed.)
