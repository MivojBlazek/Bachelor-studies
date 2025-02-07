<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\Group;
use App\Models\User;

class GroupSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $adminUser = User::where('username', 'admin')->first();

        Group::factory()->create([
            'name' => 'Test Group',
            'adminUserId' => $adminUser->userId,
        ]);
    }
}
