<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\Control;
use App\Enums\RefereeRole;
use App\Models\Game;
use App\Models\Referee;

class ControlSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $game1 = Game::first();
        $game2 = Game::where('league', 'Chanceliga')->first();
        $referee1 = Referee::first();
        $referee2 = Referee::where('username', 'rozhodci2')->first();
        $referee3 = Referee::where('username', 'rozhodci3')->first();

        Control::factory()->create([
            'refereeRole' => RefereeRole::Line,
            'feedback' => 'Nebylo to spatne',
            'game_id' => $game1->id,
            'referee_id' => $referee1->id,
        ]);

        Control::factory()->create([
            'refereeRole' => RefereeRole::Main,
            'feedback' => '',
            'game_id' => $game1->id,
            'referee_id' => $referee2->id,
        ]);

        Control::factory()->create([
            'refereeRole' => RefereeRole::Line,
            'feedback' => '',
            'game_id' => $game1->id,
            'referee_id' => $referee3->id,
        ]);

        Control::factory()->create([
            'refereeRole' => RefereeRole::Main,
            'feedback' => '',
            'game_id' => $game2->id,
            'referee_id' => $referee2->id,
        ]);
    }
}
