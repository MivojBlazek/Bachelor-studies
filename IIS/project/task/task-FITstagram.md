# Zadání není finální.

Úkolem je vytvořit informační systém, který umožní uživatelům nahrávat, sdílet, prohlížet a hodnotit fotografie, organizovat je do galerií (dle tagů) a sdílet je mezi uživateli. Každá fotografie má má nějaký název, 0..n tagů, a případné další volitelné atributy (např. popis, místo pořízení fotografie, apod.). Ke každé fotografii se váže hodnocení a komentáře jiných uživatelů. Fotografie může být buď veřejná (vidí ji všichni) nebo soukromá pro vybrané uživatele, případně skupiny. Uživatel i skupina mají svoji zeď, kde budou zobrazeny fotky daného uživatele, případně uživatelů skupiny. Systém dále umožní dalším uživatelům fotografie třídit je podle různých kritérií (nejlépe hodnocené, nejnovější, podle kategorie apod.).

* Administrátor
    * Spravuje uživatele a jejich účty.
    * Má práva všech následujících rolí.
* Moderátor
    * Odstraňuje nevhodný obsah (skupiny, fotografie, komentáře, tagy).
    * Blokuje uživatele (nemaže).
    * Má právo registrovaného uživatele
* Registrovaný uživatel (fotograf)
    * Nahrává a spravuje své fotografie.
    * Nastavuje viditelnost fotografií (veřejné/soukromé).
    * Hodnotí a komentuje fotografie ostatních uživatelů.
    * Zakládá skupiny - stává se **správcem skupiny**:
        * spravuje obsah skupiny
        * potvrzuje/přidává/odebírá uživatele
        * prohlíží skupiny
    * přihlašuje se do skupiny - po potvrzení správcem se stává **členem skupiny**
        * přidané fotografie člena skupiny **mohou** (pokud to zvolí) být zobrazeny také na zdi skupiny
* Neregistrovaný uživatel
    * prohlíží veřejně dostupné fotografie
    * nemůže nahrávat fotografie ani hodnotit nebo komentovat

## Návrhy na rozšíření:

* **notifikace**: upozornění na nové komentáře, hodnocení nebo soutěže
* **systém ocenění**: ocenění pro nejlépe hodnocené fotografie nebo uživatele (např. fotograf měsíce)
* **automatické generování miniatur** pro rychlejší prohlížení galerie
* **propojení se sociálními sítěmi**: Sdílení fotografií přímo na platformy jako Facebook, Instagram, Twitter
