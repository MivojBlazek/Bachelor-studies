# Eva setup tutorial

* remove IIS-project directory from ~/WWW/
* insert a new IIS-project directory
* update rights with:
    ```
    find ~/WWW/IIS-project -type f -exec chmod 644 {} \;
    find ~/WWW/IIS-project -type d -exec chmod 755 {} \;
    ```
* remove index.php from ~/WWW/IIS-project/public/
* update database information in ~/WWW/IIS-project/.env
    ```
    DB_CONNECTION=mysql
    DB_HOST=localhost
    DB_PORT=/var/run/mysql/mysql.sock
    DB_DATABASE=xblaze38
    DB_USERNAME=xblaze38
    DB_PASSWORD=ij5ahaso
    ```
* open [phpmyadmin](https://www.stud.fit.vutbr.cz/phpmyadmin/) on FIT VPN and run SQL command to create all the tables and insert seeded data (from migrations and seeders)
* connect to [xblaze38 website](https://www.stud.fit.vutbr.cz/~xblaze38/)

## Getting a hash of password

* on local php application run `php artisan tinker`
* to print hash use `echo bcrypt('MY_PASSWORD');`
