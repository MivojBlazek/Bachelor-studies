<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\Wall;
use App\Models\User;
use App\Models\Group;

class WallSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        User::all()->each(function ($user) {
            Wall::factory()->create([
                'userId' => $user->id,
                'groupId' => null,
            ]);
        });

        Group::all()->each(function ($group) {
            Wall::factory()->create([
                'userId' => null,
                'groupId' => $group->id,
            ]);
        });
    }
}
