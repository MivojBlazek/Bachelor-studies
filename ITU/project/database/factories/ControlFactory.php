<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Enums\RefereeRole;
use App\Models\Game;
use App\Models\Referee;

/**
 * @extends \Illuminate\Database\Eloquent\Factories\Factory<\App\Models\Control>
 */
class ControlFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array<string, mixed>
     */
    public function definition(): array
    {
        return [
            'refereeRole' => RefereeRole::Main,
            'feedback' => fake()->text(),
            'game_id' => Game::factory(),
            'referee_id' => Referee::factory(),
        ];
    }
}
