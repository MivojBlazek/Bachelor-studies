<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\Post;
use App\Models\User;

class PostSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $user = User::where('username', 'testUser')->first();
        $user2 = User::where('username', 'moderator')->first();

        Post::factory()->create([
            'photo' => 'https://plus.unsplash.com/premium_photo-1664474619075-644dd191935f?fm=jpg&q=60&w=3000&ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxzZWFyY2h8MXx8aW1hZ2V8ZW58MHx8MHx8fDA%3D',
            'description' => 'This is just test description',
            'date' => now(),
            'postCreatorId' => $user->userId,
            'visibility' => 'public',
        ]);
        Post::factory()->create([
            'photo' => 'https://images.unsplash.com/photo-1726610930930-0e1af5f2d038?q=80&w=2071&auto=format&fit=crop&ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D',
            'description' => 'This is just test description 2',
            'date' => now(),
            'postCreatorId' => $user2->userId,
            'visibility' => 'public',
        ]);
    }
}
