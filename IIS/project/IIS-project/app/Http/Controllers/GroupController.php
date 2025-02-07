<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use App\Models\Group;
use App\Models\User;
use App\Models\UserInGroup;

class GroupController extends Controller
{
    public function showPendingRequests(Request $request, $groupId)
    {
        $user = Auth::user();
        $group = Group::with(['userAsAdmin', 'wall', 'users'])->where('groupId', $groupId)->first();
        $pendingUsersInGroup = UserInGroup::where('groupId', $groupId)->where('approved', 'no')->get();

        $pendingUsers = [];
        foreach ($pendingUsersInGroup as $userInGroup)
        {
            $pendingUsers[] = User::find($userInGroup->userId);
        }

        return view('pendingRequests', compact('user', 'group', 'pendingUsers'));
    }

    public function approvePendingRequests(Request $request)
    {
        $request->validate([
            'groupId' => 'required|exists:groups,groupId',
            'userId' => 'required|exists:users,userId',
        ]);
        $pendingUser = UserInGroup::where('groupId', $request->groupId)->where('userId', $request->userId)->first();
        if ($pendingUser && $pendingUser->approved !== 'yes')
        {
            $pendingUser->approved = 'yes';
            $pendingUser->save();
        }
        return redirect()->back()->with('success', 'User approved successfully.');
    }

    public function refusePendingRequests(Request $request)
    {
        $request->validate([
            'groupId' => 'required|exists:groups,groupId',
            'userId' => 'required|exists:users,userId',
        ]);
        $pendingUser = UserInGroup::where('groupId', $request->groupId)->where('userId', $request->userId)->first();
        if ($pendingUser)
        {
            $pendingUser->delete();
        }
        return redirect()->back()->with('success', 'User refused successfully.');
    }

    public function addGroup(Request $request)
    {
        $user = Auth::user();

        $request->validate([
            'name' => 'required|string',
        ]);

        $newGroup = Group::create([
            'adminUserId' => $user->userId,
            'name' => $request->input('name'),
        ]);
        UserInGroup::create([
            'userId' => $user->userId,
            'groupId' => $newGroup->groupId,
            'approved' => 'yes',
        ]);
        
        return redirect()->back()->with('success', 'Group created successfully.');
    }

    public function showMyGroups()
    {
        $user = Auth::user();

        $adminGroups = $user->adminGroups()->with(['userAsAdmin', 'wall', 'users'])->get();
        $memberGroups = $user->memberGroups()->with(['userAsAdmin', 'wall', 'users'])->get();

        $groups = $adminGroups->merge($memberGroups);

        foreach ($groups as $group)
        {
            $group->setAttribute('isInGroup', $group->users->contains($user));
            $isInGroup = UserInGroup::where('groupId', $group->groupId)->where('userId', $user->userId)->where('approved', 'yes')->exists();
            $group->isInGroup = $isInGroup;
        }

        return view('myGroups', compact('user', 'groups'));
    }

    public function showGroups(Request $request)
    {
        $user = Auth::user();

        $request->validate([
            'name' => 'required|string|min:1',
        ]);

        $groups = Group::with(['userAsAdmin', 'wall', 'users'])->where('name', 'like', '%' . $request->name . '%')->get();

        foreach ($groups as $group)
        {
            $group->setAttribute('isInGroup', $group->users->contains($user));
        }

        return response()->json($groups, 200);
    }

    public function joinToGroup(Request $request)
    {
        $user = Auth::user();

        $request->validate([
            'groupId' => 'required|exists:groups,groupId',
        ]);

        $existingMember = UserInGroup::where('userId', $user->userId)->where('groupId', $request->groupId)->first();
        if (!$existingMember)
        {
            UserInGroup::create([
                'userId' => $user->userId,
                'groupId' => $request->groupId,
                'approved' => 'no',
            ]);
        }
        return redirect()->back()->with('success', 'Joined to group successfully.');
    }

    public function leaveGroup(Request $request)
    {
        $user = Auth::user();

        $request->validate([
            'groupId' => 'required|exists:groups,groupId',
        ]);

        $member = UserInGroup::where('userId', $user->userId)->where('groupId', $request->groupId)->first();
        if ($member)
        {
            $member->delete();
        }
        return redirect()->back()->with('success', 'Leaved group successfully.');
    }

    public function removeGroup(Request $request)
    {
        $request->validate([
            'groupId' => 'required|exists:groups,groupId',
        ]);

        $group = Group::where('groupId', $request->groupId)->first();
        if ($group)
        {
            $group->delete();
        }
        return redirect()->back()->with('success', 'Group removed successfully.');
    }
}
