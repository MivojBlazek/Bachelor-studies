<?php

namespace App\Http\Controllers;

use App\Models\Post;
use App\Models\PostVisibility;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use App\Models\User;
use Illuminate\Support\Facades\Hash;

class ProfileController extends Controller
{
    public function showUser(Request $request, $username = null)
    {
        $user = Auth::user();
        if ($username)
        {
            $viewedUser = User::where('username', $username)->first();
        }
        else
        {
            $viewedUser = $user;
        }
        // Public posts
        $publicPosts = Post::with(['creator', 'likes', 'comments'])->where('visibility', 'public')->where('postCreatorId', $viewedUser->userId)->get();
        $privatePosts = Post::with(['creator', 'likes', 'comments'])->where('visibility', 'private')->where('postCreatorId', $viewedUser->userId)->get();

        // Visible posts
        $visiblePrivate = [];
        if ($user->rights == 'admin' || $user->rights == 'moderator')
        {
            $visiblePrivate = $privatePosts;
        }
        else
        {
            foreach ($privatePosts as $privatePost)
            {
                $visibility = PostVisibility::where('postId', $privatePost->postId)->where('userId', $user->userId)->exists();
                if ($visibility)
                {
                    $visiblePrivate[] = $privatePost;
                }
            }
        }

        // Merge
        $posts = $publicPosts->merge($visiblePrivate);
        return view('profile', compact('user', 'viewedUser', 'posts'));
    }

    public function edit()
    {
        $user = Auth::user();
        return view('edit-profile', compact('user'));
    }

    public function editData(Request $request)
    {
        $user = Auth::user();

        $request->validate([
            'name' => 'required|string|max:255',
            'email' => 'required|string|email|max:255',
            'password' => 'nullable|string|confirmed',
            'age' => 'nullable|integer|min:1|max:150',
            'bio' => 'nullable|string',
            'profile_photo' => 'required|string',
        ],
        [
            'password.confirmed' => 'The password and confirmation password do not match.',
        ]);

        $user->name = $request->name;
        $user->email = $request->email;
        if ($request->filled('password'))
        {
            $user->password = Hash::make($request->password);
        }
        $user->age = $request->age ?? 0;
        $user->bio = $request->bio ?? '';
        $user->profile_photo = $request->profile_photo;
        $user->save();

        return redirect()->route('profile', ['username' => $user->username])->with('success', 'User information updated successfully.');
    }
}
