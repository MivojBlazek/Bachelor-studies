<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\PostWall;
use App\Models\Wall;
use App\Models\Post;

class PostWallSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $mainWall = Wall::all()->first();
        $posts = Post::all();

        foreach ($posts as $post)
        {
            PostWall::factory()->create([
                'wallId' => $mainWall->wallId,
                'postId' => $post->postId,
            ]);
        }
    }
}
