<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\TagsInPost;
use App\Models\Tag;
use App\Models\Post;

class TagsInPostSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $tag = Tag::where('name', 'testTag')->first();
        $post = Post::all()->first();

        TagsInPost::factory()->create([
            'tagId' => $tag->tagId,
            'postId' => $post->postId,
        ]);
    }
}
