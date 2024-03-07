# ER diagram

Jelikož zadání vyžaduje evidenci řidičů a motorových vozidel, máme v ER diagramu entitní množiny Řidič a Motorové vozidlo. Entitní množina Motorové vozidlo obsahuje VIN motorového mozidla jako primární klíč, jeho značku, SPZ, rok výroby a řidičskou skupina, která je potřebná pro řízení tohoto vozidla. Motorové vozidlo může být kradené, což je speciální typ motorového vozidla, který má navíc atributy datum krádeže a místo krádeže.

Systém také eviduje přestupky a o nich eviduje jejich název, popis a maximální a minimální výši pokuty, kterou lze za tento přestupek udělit řidiči. Pro evidování přestupků, které určitý řidič spáchal, máme entitní množinu Spáchal. Tato entitní množina obsahuje rodné číslo určitého řidiče a ID přestupku, který řidič spáchal. Dále obsahuje datum spáchání přestupku, popis a výši pokuty, která byla řidiči udělena.

Entitní množina Řidič obsahuje pouze řidičské opravnění, protože zbytek atributů je zděden z entitní množiny Osoba, kde je primární klíč rodné číslo osoby. Dále tu jsou atributy jméno, příjmení, trvalý pobyt osoby a telefonní číslo.

Entitní množina Vlastník v tomto ER diagramu slouží pro uchování informace o osobě nebo firmě, která vlastní motorové vozidlo. Entitní množina Firma je právě tento vlastník. Její primární klíč je IČO a dalšími atributy jsou název, sídlo a telefonní číslo.

Vazba mezi entitními množinami Řidič a Spáchal je 1:N, protože řidič může spáchat více přestupků. To samé platí pro Přestupek a Spáchal, kde jeden přestupek může spáchat více řidičů. Vazba mezi řidičem a motorovým vozidlem je N:N. Řidič řídí více aut, ale také to auto můžou řídit i další řidiči. Vlastník bude ke každému motorovému vozidlu vždy jen 1 a vlastník může vlastnit i více motorových vozidel.
