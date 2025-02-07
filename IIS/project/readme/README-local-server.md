# Laravel

There is simple project in Laravel

### Login credentials

Username: testUser
Password: password

Username: admin
Password: password

Username: moderator
Password: password

## How to start server

* Start server in terminal `./vendor/bin/sail up`
* In another terminal to delete existing database from migrations `./vendor/bin/sail artisan migrate:rollback`
* In another terminal to create database from migrations `./vendor/bin/sail artisan migrate`
* Just to seed data like login credentials use `./vendor/bin/sail artisan db:seed`
* Connect to http://localhost/
* Stop server `Ctrl+C` or `./vendor/bin/sail down`

## Good to debug

* Opens mysql database `./vendor/bin/sail artisan tinker`
    * In tinker mode to show all users `\App\Models\User::all();`
    * To delete specific user according to id `\App\Models\User::where('id', 10)->delete();`

## Description of important files and directories

./app/Models/
* models

./app/Http/Controllers/
* controllers

./config/database.php
* lines 50, 51, 52 -> maybe database credentials

./database/factories/
* factories to each model
* generated using `php artisan make:factory NameOfFactory --model=NameOfModel`
* then add model default state

./database/migrations/
* migrations (just for nothing now I thing)

./database/seeders/
* seeds of all tables
* somehow can be generated but it broke my program

./resources/css/
* css files

./resources/js/
* js files

./resources/views/
* views

./routes/web.php
* all routes that can use redirect
* maybe there will be security restrictions

./tests/
* tests (someone can create if want to)

./.env
* maybe database setup
