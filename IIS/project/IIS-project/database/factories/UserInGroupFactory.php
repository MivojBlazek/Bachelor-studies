<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Models\User;
use App\Models\Group;

/**
 * @extends \Illuminate\Database\Eloquent\Factories\Factory<\App\Models\Model>
 */
class UserInGroupFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array<string, mixed>
     */
    public function definition(): array
    {
        return [
            'userInGroupId' => fake()->unique()->randomNumber(),
            'userId' => User::factory(),
            'groupId' => Group::factory(),
            'approved' => 'no',
        ];
    }
}
