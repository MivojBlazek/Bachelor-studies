<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\Comment;
use App\Models\User;
use App\Models\Post;

class CommentSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $user = User::where('username', 'admin')->first();
        $moderator = User::where('username', 'moderator')->first();
        $post = Post::where('postCreatorId', $moderator->userId)->first();

        Comment::factory()->create([
            'content' => 'Test content',
            'userId' => $user->userId,
            'postId' => $post->postId,
        ]);
    }
}
