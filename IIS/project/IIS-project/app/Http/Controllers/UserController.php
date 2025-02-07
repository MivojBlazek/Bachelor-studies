<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use App\Models\User;

class UserController extends Controller
{
    public function removeUser(Request $request)
    {
        $request->validate([
            'userId' => 'required|exists:users,userId',
        ]);

        $existingUser = User::where('userId', $request->userId)->first();
        if ($existingUser)
        {
            $existingUser->delete();
        }
        return redirect()->route('FYP')->with('success', 'User removed successfully.');
    }

    public function banUser(Request $request)
    {
        $request->validate([
            'userId' => 'required|exists:users,userId',
        ]);

        $existingUser = User::where('userId', $request->userId)->first();
        if ($existingUser)
        {
            $existingUser->banFor(2); //TODO time to ban user
        }
        return redirect()->back()->with('success', 'User banned successfully.');
    }

    public function unbanUser(Request $request)
    {
        $request->validate([
            'userId' => 'required|exists:users,userId',
        ]);

        $existingUser = User::where('userId', $request->userId)->first();
        if ($existingUser)
        {
            $existingUser->unban();
        }
        return redirect()->back()->with('success', 'User unbanned successfully.');
    }

    public function showUsers(Request $request)
    {
        $request->validate([
            'name' => 'required|string|min:1',
        ]);

        $users = User::where('name', 'like', '%' . $request->name . '%')->get();
        return response()->json($users, 200);
    }
}
