<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Models\Game;
use App\Models\Delegate;

/**
 * @extends \Illuminate\Database\Eloquent\Factories\Factory<\App\Models\Video>
 */
class VideoFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array<string, mixed>
     */
    public function definition(): array
    {
        return [
            'url' => fake()->text(),
            'description' => fake()->text(),
            'uploadedAt' => now(),
            'game_id' => Game::factory(),
            'uploaded_by' => Delegate::factory(),
        ];
    }
}
