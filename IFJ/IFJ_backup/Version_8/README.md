Zaloha

Date: 22.11.
Time: 18:42
Contributors: xblaze38, xmicha94

<<COPY from Version_1>>
Parser bere ze stdin tokeny a dava je expressions a ty je vyhodnocuji jako vyraz. Pokud je vse ok a neni tam zadna chyba ve vyrazu, tak nakonec na stderr dostaneme poradi pravidel jak jdou po sobe. Je to zatim jen pro DEBUG z stdin do stderr. Obsahuje to jeste 2 zasobniky (1 na tokeny a 1 na polozky, ale ten jeste nebyl pouzit). Scanner i parser maji stale jen zakladni struktury jako je Token a parserData, aby expressions dostaly spravna data. Valgrind pro ./parser nehazi zadne problemy ani leaknutou pamet.
<</COPY from Version_1>>

<<COPY from Version_2>>
Scanner nyni implementovan. Stale nebyl jeste nejak moc testovan a jsou tam bugy. Napriklad ma problem ulozit do tokenu konkretne "write". Valgrind ma nejaky maly mouchy, take nebylo moc testovano. Dale byl upraven Makefile, aby vzdy vymazal stavajici soubory, takze uz se nemusi volat make clean pred make scanner/parser/stack. Samozrejme byl pripraven i test v mainu pro testovani scanneru.
Vetsinou to bylo spousteno jako ./scanner < cat ../priklady/example[1/2].swift.
<</COPY from Version_2>>

<<COPY from Version_3>>
Scanner prosel nekolika malo testy a je funkcni. Valgrind taky nema zadny problem. Main je aktualne postaven pro testovani scanneru, ale samozrejme i test zasobniku a parseru funguje. Trosku se zmenil konecny automat scanneru, takze by bylo fajn udelat novy a hodit ho tam (nejlepe aby byl uz v dia).
<</COPY from Version_3>>

<<COPY from Version_4>>
Zalozen symtable. Prosel nekolika verzema, vcetne te s aktivnim priznakem, ale finalni verze je klasicky rozdelena na globalni a lokalni tabulky symbolu. Nebyl nijak testovan, takze je dost mozne, ze symtable stale jeste nefunguje. Scanner opraveno par bugu a testovano. Scanner funguje jak ma, jeste si hraju s myslenkou Int a Double nedavat zvlast do attribute.i a attribute.d, ale nechat to v textu (attribute.s). Main klasicke zmeny, kde se jen testuje. Pridam konecny automat z dia pro scanner a LL tabulka a LL gramatika pro parser. Ve stack.c byl opraven zasobnik s polozkami (itemStack) na zasobnik programu (programStack), ktery se bude vyuzivat v parseru. Parser byl malinko zacat ohledne hlavicky funkce, ale potom mi doslo, ze to mame delat pres LL gramatiku a ne pres if-else, tak je to pouze zakomentovany a chce to vzit si z toho inspiraci a udelat podle toho tu verzi s LL gramatikou.
<</COPY from Version_4>>

<<COPY from Version_5>>
Opravil jsem par bugu v symtablu a mainu, ale hlavne vylepsil parserData strukturu, ktera slouzi pro predavani dat mezi funkcemi, protoze se volaji rekurzivne. Mozna by nebylo od veci dat k funkcim static. Hlavne tyto funkce tvorici dohromady ll-gramatiku parseru, jsem implementoval a cekaji na testovani.
<</COPY from Version_5>>

<<COPY from Version_6>>
Aktualne byl hlavni cil zprovoznit parser a expressions, aby uz konecne sel prelozit spravny program a spadnul chybny program. Expressions se volaji pouze na mistech expression() a valueExpression(), coz by snad melo pokryt veskerou praci s nimi. Parser take uz funguje krasne a projde to vsemi kodu mimo substr.swift, kde je vyraz x ?? x rozdelen na vice radku, coz rikali, ze nebudou drasticky testovat. Nyni je na rade implementovat lokalni tabulky symbolu (globalni TS snad uz funguje) a vyuzivat je v kodu. Parser ma v sobe nejake TODO, kde je napsano, ze by mohl kontrolovat typy apod. Typova kontrola je samozrejme namiste hned, co se dodela lokalni TS.
<</COPY from Version_6>>

<<COPY from Version_7>>
Tento funkcni kod jsem postupem casu dost rozbil a nasledne se snazil opravit. Cilem bylo zprovoznit lokalni tabulky symbolu a kontrolovat datove typy na vsech moznych mistech. Datove typy se kontroluji uz skoro vsude, ale neni to uplne funkcni kod. Lokalni tabulka je implementovana pomoci statickeho pole o 255 prvcich, coz se snad neprekroci (dalo by se to implementovat nejak pres zasobnik, ktery Krystof uz pripravil). Kod byl testovan na nekolika prikladech, pricemz z tech dulezitejsich example[1,2].swift to proslo v poradku. Jsou tam chybicky a chce to oddebugovat, a pote samozrejme zkontrolovat i navratove hodnoty, pokud prekladac najde chybu v kodu.
<</COPY from Version_7>>

Tato verze oddebugovala nejake posledni detaily z verze 7. Parser uz pracuje s vetsinou datovych typu. Expression taky uz neco malo rozeznaji. Byly upraveny navratove hodnoty na snad spravne podle zadani. Uz byla tato verze i pripravena na pokusne odevzdani a testovana pomoci is_it_ok.sh.
