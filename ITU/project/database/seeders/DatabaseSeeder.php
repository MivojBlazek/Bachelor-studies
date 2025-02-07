<?php

namespace Database\Seeders;

// use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

class DatabaseSeeder extends Seeder
{
    /**
     * Seed the application's database.
     */
    public function run(): void
    {
        $this->call([
            DelegateSeeder::class,
            RefereeSeeder::class,
            ClubSeeder::class,
            GameSeeder::class,
            ControlSeeder::class,
            VideoSeeder::class,
            PaymentSeeder::class,
            PlayerSeeder::class,
        ]);
    }
}
