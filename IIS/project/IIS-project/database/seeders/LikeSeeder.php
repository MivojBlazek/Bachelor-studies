<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\Like;
use App\Models\User;
use App\Models\Post;

class LikeSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $user = User::where('username', 'testUser')->first();
        $moderator = User::where('username', 'moderator')->first();
        $post = Post::where('postCreatorId', $moderator->userId)->first();

        Like::factory()->create([
            'userId' => $user->userId,
            'postId' => $post->postId,
        ]);
    }
}
