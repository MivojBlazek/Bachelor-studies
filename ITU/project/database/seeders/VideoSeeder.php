<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\Video;
use App\Models\Delegate;
use App\Models\Game;

class VideoSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $game = Game::first();
        $delegate = Delegate::first();

        Video::factory()->create([
            'url' => 'www.google.com',
            'description' => 'Hráč číslo 33 z týmu domácích spadl sám a dostal 2 minuty za podražení sebe.',
            'uploadedAt' => now(),
            'game_id' => $game->id,
            'uploaded_by' => $delegate->id,
        ]);

        Video::factory()->create([
            'url' => 'URL tady',
            'description' => '',
            'uploadedAt' => now(),
            'game_id' => $game->id,
            'uploaded_by' => $delegate->id,
        ]);
    }
}
