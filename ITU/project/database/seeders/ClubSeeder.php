<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\Club;

class ClubSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        Club::factory()->create([
            'name' => 'Klub',
            'username' => 'klub',
            'password' => bcrypt('password'),
            'city' => 'Brno',
            'address' => 'Božetěchova 1',
            'bank_account' => '123456789/1234',
            'phone' => '111 222 333',
            'email' => 'klub@gmail.com',
        ]);

        Club::factory()->create([
            'name' => 'Pražané',
            'username' => 'klub2',
            'password' => bcrypt('password'),
            'city' => 'Praha',
            'address' => 'Václavské náměstí 1',
            'bank_account' => '10001000/1234',
            'phone' => '123 456 789',
            'email' => 'klub2@gmail.com',
        ]);

        Club::factory()->create([
            'name' => 'Kometa',
            'username' => 'klub3',
            'password' => bcrypt('password'),
            'city' => 'Brno',
            'address' => 'Technická 10',
            'bank_account' => '19283746/1234',
            'photo' => 'https://i.pinimg.com/736x/8b/16/7a/8b167af653c2399dd93b952a48740620.jpg',
            'phone' => '111 111 111',
            'email' => 'kometa@brno.cz',
        ]);
    }
}
