<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Models\Wall;
use App\Models\Post;
use App\Models\User;
use App\Models\Group;

/**
 * @extends \Illuminate\Database\Eloquent\Factories\Factory<\App\Models\Wall>
 */
class WallFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array<string, mixed>
     */
    public function definition(): array
    {
        if (fake()->boolean)
        {
            return [
                'wallId' => fake()->unique()->randomNumber(),
                'userId' => User::factory(),
                'groupId' => null,
            ];
        }
        return [
            'wallId' => fake()->unique()->randomNumber(),
            'userId' => null,
            'groupId' => Group::factory(),
        ];
    }
}
