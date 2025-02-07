<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\Referee;

class RefereeSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        Referee::factory()->create([
            'name' => 'Rozhodci',
            'username' => 'rozhodci',
            'license' => 'B2',
            'address' => 'Božetěchova 3',
            'bank_account' => '11112222/1234',
            'password' => bcrypt('password'),
            'birth_date' => today()->subYears(5),
            'phone' => '999 888 777',
            'email' => 'rozhodci@gmail.com',
        ]);

        Referee::factory()->create([
            'name' => 'Matyáš Sapík',
            'username' => 'rozhodci2',
            'license' => 'B4',
            'address' => 'Božetěchova 17',
            'bank_account' => '14848482/1234',
            'password' => bcrypt('password'),
            'birth_date' => today()->subYears(21),
            'phone' => '774 786 229',
            'email' => 'matyas.sapik@gmail.com',
        ]);

        Referee::factory()->create([
            'name' => 'Matěj Lepeška',
            'username' => 'rozhodci3',
            'license' => 'BD9',
            'address' => 'Technická 107/17',
            'bank_account' => '11119999/1234',
            'password' => bcrypt('password'),
            'photo' => 'https://i.pinimg.com/736x/8b/16/7a/8b167af653c2399dd93b952a48740620.jpg',
            'birth_date' => today()->subYears(21),
            'phone' => '723 845 567',
            'email' => 'matej.lepeska@gmail.com',
        ]);
    }
}
