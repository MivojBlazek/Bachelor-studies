<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\User;

class UserSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        User::factory()->create([
            'name' => 'Test User',
            'username' => 'testUser',
            'email' => 'user@example.com',
            'password' => bcrypt('password'),
            'age' => 22,
            'rights' => 'user',
            'profile_photo' => 'https://www.perfocal.com/blog/content/images/2021/01/Perfocal_17-11-2019_TYWFAQ_100_standard-3.jpg',
            'banned_until' => null,
            'bio' => '',
        ]);
        User::factory()->create([
            'name' => 'Administrator',
            'username' => 'admin',
            'email' => 'admin@example.com',
            'password' => bcrypt('password'),
            'age' => 20,
            'rights' => 'admin',
            'profile_photo' => 'https://images.ctfassets.net/h6goo9gw1hh6/2sNZtFAWOdP1lmQ33VwRN3/24e953b920a9cd0ff2e1d587742a2472/1-intro-photo-final.jpg?w=1200&h=992&fl=progressive&q=70&fm=jpg',
            'banned_until' => null,
            'bio' => 'Bio',
        ]);
        User::factory()->create([
            'name' => 'Moderator',
            'username' => 'moderator',
            'email' => 'moderator@example.com',
            'password' => bcrypt('password'),
            'age' => 21,
            'rights' => 'moderator',
            'profile_photo' => 'https://cdn.pixabay.com/photo/2015/10/05/22/37/blank-profile-picture-973460_1280.png',
            'banned_until' => null,
            'bio' => 'Bio here',
        ]);
    }
}
