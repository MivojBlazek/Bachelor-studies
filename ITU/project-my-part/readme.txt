Adresářová struktura projektu


app - Enums - Obsahuje definice konstantního výčtu prvků (například hráči můžou být útočníci, obránci a brankáři)
    - Http - Controllers - Obsahuje kontrolery, které zpracovávají požadavky frontendu (* rozdělené podle role uživatele)
           - Requests - Validuje data přijatých požadavků z frontendu (* rozdělené podle role uživatele)
    - Models - Obsahuje modely aplikace, které zobrazují databázové tabulky
database - factories - Generátory dat pro naplnění tabulek.
         - migrations - Obsahuje migrace pro vytváření tabulek a mazání tabulek
         - seeders - Obsahuje třídy pro naplnění tabulek daty
routes - api.php - Slouží pro komunikaci mezi backendem a frontendem pomocí definovaných API endpointů
ITU-project - index.html - Základní HTML soubor, který se spouští s otevřením aplikace
            - src - router.jsx - Soubor obsahuje definici cest a k tomu určuje jaký pohled se má renderovat na jaké cestě
                  - axiosClient.jsx - konfigurace Axios klienta pro HTTP požadavky
                  - colors.jsx - Definice jednotných barev pro projekt
                  - main.jsx - Pomocí aktuální cesty rozšiřuje index.html
                  - Components - Obsahuje komponenty aplikace využívané pohledy (* rozdělené podle role uživatele)
                  - contexts - contextprovider.jsx - 
                  - Layouts - Obsahuje rozložení pro každou roli a například i pro nepřihlášeného uživatele
                  - views - Obsahuje pohledy aplikace (* rozdělené podle role uživatele)


* Všechny soubory rozdělené do adresářů delegate/, referee/ a club/ jsou implementovány pro každou roli zvlášť a každou roli implementoval jiný člen týmu.

Delegate - Michal Blažek
Referee - Matěj Lepeška
Club - Matyáš Sapík
