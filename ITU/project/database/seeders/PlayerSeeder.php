<?php

namespace Database\Seeders;

use App\Enums\PlayerRole;
use App\Models\Club;
use App\Models\Player;
use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

class PlayerSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $club1 = Club::first();
        $club2 = Club::where('username', 'klub2')->first();

        Player::factory()->create([
            'name' => 'Castor Brackenwall',
            'dress_number' => 51,
            'role' => PlayerRole::Forward,
            'address' => 'Božetěchova 34',
            'phone' => '879 587 184',
            'email' => 'hrac1@gmail.com',
            'club_id' => $club1->id,
        ]);

        Player::factory()->create([
            'name' => 'Ian Domongart Wenlock',
            'dress_number' => 67,
            'role' => PlayerRole::Defend,
            'address' => 'Božetěchova 35',
            'phone' => '999 000 033',
            'email' => 'hrac2@gmail.com',
            'club_id' => $club1->id,
        ]);

        Player::factory()->create([
            'name' => 'Winstone Brunor Bourchier',
            'dress_number' => 73,
            'role' => PlayerRole::Goalkeeper,
            'address' => 'Božetěchova 36',
            'phone' => '999 000 003',
            'email' => 'hrac3@gmail.com',
            'club_id' => $club2->id,
        ]);
    }
}
