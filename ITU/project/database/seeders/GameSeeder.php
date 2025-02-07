<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\Game;
use App\Models\Club;
use App\Models\Delegate;

class GameSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $club1 = Club::first();
        $delegate = Delegate::first();
        $delegate2 = Delegate::where('username', 'delegat2')->first();
        $club2 = Club::where('username', 'klub2')->first();
        $club3 = Club::where('username', 'klub3')->first();

        Game::factory()->create([
            'date' => today(),
            'time' => now(),
            'location' => $club1->city,
            'league' => 'Extraliga',
            'clubHome_id' => $club1->id,
            'clubVisitor_id' => $club2->id,
            'delegate_id' => $delegate->id,
        ]);

        Game::factory()->create([
            'date' => today(),
            'time' => now(),
            'location' => $club2->city,
            'league' => 'Extraliga',
            'clubHome_id' => $club2->id,
            'clubVisitor_id' => $club3->id,
            'delegate_id' => null,
        ]);

        Game::factory()->create([
            'date' => today()->addDays(20),
            'time' => now(),
            'location' => $club1->city,
            'league' => 'Chanceliga',
            'clubHome_id' => $club1->id,
            'clubVisitor_id' => $club2->id,
            'delegate_id' => $delegate->id,
        ]);

        Game::factory()->create([
            'date' => today()->addDays(20),
            'time' => now(),
            'location' => $club1->city,
            'league' => 'Hobby',
            'clubHome_id' => $club1->id,
            'clubVisitor_id' => $club3->id,
            'delegate_id' => $delegate2->id,
        ]);

        Game::factory()->create([
            'date' => today()->addDays(50),
            'time' => now(),
            'location' => $club3->city,
            'league' => 'NHL',
            'clubHome_id' => $club3->id,
            'clubVisitor_id' => $club2->id,
            'delegate_id' => null,
        ]);
    }
}
