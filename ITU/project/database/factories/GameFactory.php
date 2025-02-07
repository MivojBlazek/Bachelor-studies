<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Models\Delegate;
use App\Models\Club;

/**
 * @extends \Illuminate\Database\Eloquent\Factories\Factory<\App\Models\Game>
 */
class GameFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array<string, mixed>
     */
    public function definition(): array
    {
        return [
            'date' => fake()->date(),
            'time' => fake()->time(),
            'location' => fake()->text(),
            'league' => fake()->text(),
            'clubHome_id' => Club::factory(),
            'clubVisitor_id' => Club::factory(),
            'delegate_id' => Delegate::factory(),
        ];
    }
}
