<?php

namespace Database\Factories;

use App\Enums\PlayerRole;
use App\Models\Club;
use Illuminate\Database\Eloquent\Factories\Factory;

/**
 * @extends \Illuminate\Database\Eloquent\Factories\Factory<\App\Models\Player>
 */
class PlayerFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array<string, mixed>
     */
    public function definition(): array
    {
        return [
            'name' => fake()->name(),
            'dress_number' => 0,
            'role' => PlayerRole::Forward,
            'address' => fake()->text(),
            'phone' => fake()->phoneNumber(),
            'email' => fake()->email(),
            'club_id' => Club::factory(),
        ];
    }
}
