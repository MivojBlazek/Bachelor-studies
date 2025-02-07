<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\Delegate;

class DelegateSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        Delegate::factory()->create([
            'name' => 'Delegat',
            'username' => 'delegat',
            'license' => 'A',
            'address' => 'Božetěchova 2',
            'bank_account' => '11223344/1122',
            'password' => bcrypt('password'),
            'photo' => 'https://i.pinimg.com/736x/8b/16/7a/8b167af653c2399dd93b952a48740620.jpg',
            'birth_date' => today()->subYears(20),
            'phone' => '111 222 333',
            'email' => 'delegat@gmail.com',
        ]);

        Delegate::factory()->create([
            'name' => 'Michal Blažek',
            'username' => 'delegat2',
            'license' => 'AB1',
            'address' => 'Božetěchova 22',
            'bank_account' => '10000009/1122',
            'password' => bcrypt('password'),
            'birth_date' => today()->subYears(40),
            'phone' => '100 000 001',
            'email' => 'mb@gmail.com',
        ]);

        Delegate::factory()->create([
            'name' => 'Karel J.',
            'username' => 'delegat3',
            'license' => 'C',
            'address' => 'Technická 8',
            'bank_account' => '55500555/4122',
            'password' => bcrypt('password'),
            'birth_date' => today()->subYears(2),
            'phone' => '420 420 420',
            'email' => 'kaja@gmail.com',
        ]);
    }
}
