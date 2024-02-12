Zaloha

Date: 23.10.
Time: 11:23
Contributors: xblaze38

<<COPY from Version_1>>
Parser bere ze stdin tokeny a dava je expressions a ty je vyhodnocuji jako vyraz. Pokud je vse 		ok a neni tam zadna chyba ve vyrazu, tak nakonec na stderr dostaneme poradi pravidel jak jdou po sobe. Je to zatim jen pro DEBUG z stdin do stderr. Obsahuje to jeste 2 zasobniky (1 na tokeny a 1 na polozky, ale ten jeste nebyl pouzit). Scanner i parser maji stale jen zakladni struktury jako je Token a parserData, aby expressions dostaly spravna data. Valgrind pro ./parser nehazi zadne problemy ani leaknutou pamet.
<</COPY from Version_1>>

<<COPY from Version_2>>
Scanner nyni implementovan. Stale nebyl jeste nejak moc testovan a jsou tam bugy. Napriklad ma problem ulozit do tokenu konkretne "write". Valgrind ma nejaky maly mouchy, take nebylo moc testovano. Dale byl upraven Makefile, aby vzdy vymazal stavajici soubory, takze uz se nemusi volat make clean pred make scanner/parser/stack. Samozrejme byl pripraven i test v mainu pro testovani scanneru.
Vetsinou to bylo spousteno jako ./scanner < cat ../priklady/example[1/2].swift.
<</COPY from Version_2>>

Scanner prosel nekolika malo testy a je funkcni. Valgrind taky nema zadny problem. Main je aktualne postaven pro testovani scanneru, ale samozrejme i test zasobniku a parseru funguje. Trosku se zmenil konecny automat scanneru, takze by bylo fajn udelat novy a hodit ho tam (nejlepe aby byl uz v dia).
