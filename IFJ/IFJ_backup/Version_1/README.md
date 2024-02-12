Zaloha

Date: 20.10.
Time: 13:00
Contributors: xblaze38

Parser bere ze stdin tokeny a dava je expressions a ty je vyhodnocuji jako vyraz. Pokud je vse ok a neni tam zadna chyba ve vyrazu, tak nakonec na stderr dostaneme poradi pravidel jak jdou po sobe. Je to zatim jen pro DEBUG z stdin do stderr. Obsahuje to jeste 2 zasobniky (1 na tokeny a 1 na polozky, ale ten jeste nebyl pouzit). Scanner i parser maji stale jen zakladni struktury jako je Token a parserData, aby expressions dostaly spravna data. Valgrind pro ./parser nehazi zadne problemy ani leaknutou pamet.
