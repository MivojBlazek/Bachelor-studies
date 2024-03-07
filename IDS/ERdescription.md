# ER diagram

Jelikož zadání vyžaduje evidenci řidičů a motorových vozidel, máme v ER diagramu entitní množiny Řidič a Motorové vozidlo. Entitní množina Motorové vozidlo obsahuje VIN motorového mozidla jako primární klíč, jeho značku, SPZ, rok výroby a řidičskou skupina, která je potřebná pro řízení tohoto vozidla. Motorové vozidlo může být kradené, což je speciální typ motorového vozidla, který má navíc atributy datum krádeže a místo krádeže.

Systém také eviduje přestupky a o nich eviduje jejich název, popis a maximální a minimální výši pokuty, kterou lze za tento přestupek udělit řidiči. Pro evidování přestupků, které určitý řidič spáchal, máme entitní množinu Spáchal. Tato entitní množina obsahuje rodné číslo určitého řidiče a ID přestupku, který řidič spáchal. Dále obsahuje datum spáchání přestupku, popis a výši pokuty, která byla řidiči udělena.

