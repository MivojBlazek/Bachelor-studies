<?php

namespace Database\Seeders;

// use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

class DatabaseSeeder extends Seeder
{
    /**
     * Seed the application's database.
     */
    public function run(): void
    {
        $this->call([
            UserSeeder::class,
            GroupSeeder::class,
            UserInGroupSeeder::class,
            PostSeeder::class,
            WallSeeder::class,
            PostWallSeeder::class,
            LikeSeeder::class,
            CommentSeeder::class,
            TagSeeder::class,
            TagsInPostSeeder::class,
            PostVisibilitySeeder::class,
        ]);
    }
}
