<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Models\Wall;
use App\Models\Post;

/**
 * @extends \Illuminate\Database\Eloquent\Factories\Factory<\App\Models\PostWall>
 */
class PostWallFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array<string, mixed>
     */
    public function definition(): array
    {
        return [
            'postWallId' => fake()->unique()->randomNumber(),
            'wallId' => Wall::factory(),
            'postId' => Post::factory(),
        ];
    }
}
