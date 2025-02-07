<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Models\Tag;
use App\Models\Post;

/**
 * @extends \Illuminate\Database\Eloquent\Factories\Factory<\App\Models\TagsInPost>
 */
class TagsInPostFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array<string, mixed>
     */
    public function definition(): array
    {
        return [
            'tagsInPostId' => fake()->unique()->randomNumber(),
            'tagId' => Tag::factory(),
            'postId' => Post::factory(),
        ];
    }
}
