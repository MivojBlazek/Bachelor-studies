<?php

namespace Database\Factories;

use App\Models\Post;
use App\Models\User;
use Illuminate\Database\Eloquent\Factories\Factory;

/**
 * @extends \Illuminate\Database\Eloquent\Factories\Factory<\App\Models\PostVisibility>
 */
class PostVisibilityFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array<string, mixed>
     */
    public function definition(): array
    {
        return [
            'postVisibilityId' => fake()->unique()->randomNumber(),
            'userId' => User::factory(),
            'postId' => Post::factory(),
        ];
    }
}
