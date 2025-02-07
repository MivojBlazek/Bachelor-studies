<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Models\User;

/**
 * @extends \Illuminate\Database\Eloquent\Factories\Factory<\App\Models\Post>
 */
class PostFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array<string, mixed>
     */
    public function definition(): array
    {
        return [
            'photo' => fake()->imageUrl(),
            'description' => fake()->text(255),
            'place' => fake()->city(),
            'date' => now(),
            'postCreatorId' => User::factory(),
            'visibility' => 'public',
        ];
    }
}
