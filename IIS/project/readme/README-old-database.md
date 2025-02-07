<!---------------->
<!-- DEPRECATED -->
<!-- DO NOT USE -->
<!---------------->

# Database operations (DEPRECATED, DO NOT USE)

## Creating database from database.sql

```
sudo apt install mysql-server

    // Prihlaseni do MySQL prostredi (staci dat Enter bez hesla)
sudo mysql -u root -p

    // Vytvori databazi xblaze38 (jako na eve)
CREATE DATABASE xblaze38;

    // Vytvori uzivatele xblaze38 (jako na eve) se stejnym heslem a da mu prava
CREATE USER 'xblaze38'@'localhost' IDENTIFIED BY 'ij5ahaso';
GRANT ALL PRIVILEGES ON xblaze38.* TO 'xblaze38'@'localhost';
GRANT PROCESS ON *.* TO 'xblaze38'@'localhost';
FLUSH PRIVILEGES;
exit;

    // Na importuje databazi ze souboru database.sql do pocitace
    // Jedna z variant mi sla (heslo: 'ij5ahaso')
mysql -u root -p xblaze38 < /ABSOLUTE_PATH/database.sql
mysql -u xblaze38 -p xblaze38 < /ABSOLUTE_PATH/database.sql


sudo apt install libmysqlclient-dev

    // Zobrazi socket (pouziti v connection stringu)
mysql_config --socket
```

### Verify if database exists

```
    // Prihlaseni do MySQL prostredi (staci dat Enter bez hesla)
sudo mysql -u root -p

USE xblaze38;
SHOW TABLES;
exit;
```

## Saving database to database.sql

```
    // Ulozi databazi do database.sql (heslo je to k databazi 'ij5ahaso')
mysqldump -u xblaze38 -p xblaze38 > /ABSOLUTE_PATH/database.sql
```

## Updating local database with a new one from database.sql

If someone changed database.sql, I need to update my local one.

```
    // Prihlaseni do MySQL prostredi (staci dat Enter bez hesla)
sudo mysql -u root -p

    // Smaze existujici databazi xblaze38
DROP DATABASE xblaze38;

    // Vytvori databazi xblaze38 (jako na eve)
CREATE DATABASE xblaze38;

    // Pokud uz uzivatel existuje a ma prava, vytvaret se nemusi
SHOW GRANTS FOR 'xblaze38'@'localhost';
exit;

    // Na importuje databazi ze souboru database.sql do pocitace
    // Jedna z variant mi sla (heslo: 'ij5ahaso')
mysql -u root -p xblaze38 < /ABSOLUTE_PATH/database.sql
mysql -u xblaze38 -p xblaze38 < /ABSOLUTE_PATH/database.sql
```

### Verify if database exists

```
    // Prihlaseni do MySQL prostredi (staci dat Enter bez hesla)
sudo mysql -u root -p

USE xblaze38;
SHOW TABLES;
exit;
```

## Connecting to local database

If local database is created, we should be able to connect to it.

```
    // Vypise lokalni socket
mysql_config --socket

    // Zkontrolujte pouziti spravneho socketu z minuleho prikazu v .env
DB_SOCKET=/PATH/TO/LOCAL/SOCKET.sock

    // Ve slozce s projektem (soubor index.html) spustime php server
php -S localhost:8000

    // V prohlizeci se pripojime na adresu serveru
http://localhost:8000/
```

Terminal with running php server is giving us information if any button or anything else is not working correctly.