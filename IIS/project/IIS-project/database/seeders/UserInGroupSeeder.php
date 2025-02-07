<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Models\UserInGroup;
use App\Models\User;
use App\Models\Group;

class UserInGroupSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $adminUser = User::where('username', 'admin')->first();
        $group = Group::where('adminUserId', $adminUser->userId)->first();

        $user = User::where('username', 'testUser')->first();

        UserInGroup::factory()->create([
            'userId' => $user->userId,
            'groupId' => $group->groupId,
            'approved' => 'no',
        ]);

        UserInGroup::factory()->create([
            'userId' => $adminUser->userId,
            'groupId' => $group->groupId,
            'approved' => 'yes',
        ]);
    }
}
