<?php

namespace App\Http\Controllers;

use App\Models\Group;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use App\Models\Wall;
use App\Models\Post;
use App\Models\PostVisibility;
use App\Models\User;

class WallController extends Controller
{
    public function showFYP(Request $request)
    {
        $user = Auth::user();

        $sortBy = $request->input('sortBy', 'newest');

        // Public posts
        $publicPosts = Post::with(['creator', 'likes', 'comments'])->where('visibility', 'public')->orWhere('postCreatorId', $user->userId)->get();
        $privatePosts = Post::with(['creator', 'likes', 'comments'])->where('visibility', 'private')->get();

        // Visible posts
        $visiblePrivate = [];
        foreach ($privatePosts as $privatePost)
        {
            $visibility = PostVisibility::where('postId', $privatePost->postId)->where('userId', $user->userId)->exists();
            if ($visibility)
            {
                $visiblePrivate[] = $privatePost;
            }
        }

        // Merge
        $posts = $publicPosts->merge($visiblePrivate);

        switch ($sortBy)
        {
            case 'newest':
                $posts = $posts->sortByDesc('date');
                break;
            case 'oldest':
                $posts = $posts->sortBy('date');
                break;
            case 'most-likes':
                $posts = $posts->sortByDesc(function ($post) {
                    return $post->likes->count();
                });
                break;
            case 'least-likes':
                $posts = $posts->sortBy(function ($post) {
                    return $post->likes->count();
                });
                break;
            default:
                break;
        }

        return view('FYP', compact('user', 'posts'));
    }

    public function showGroupWall(Request $request, $groupId)
    {
        $user = Auth::user();

        $group = Group::with(['users' => function($query) {
            $query->wherePivot('approved', 'yes');
        }])->where('groupId', $groupId)->first();
    
        
        $posts = collect();
        foreach ($group->users as $groupUser)
        {
            $userPosts = Post::with(['creator', 'likes', 'comments'])->where('postCreatorId', $groupUser->userId)->where('visibility', 'public')->get();
            $posts = $posts->merge($userPosts);
        }

        $sortBy = $request->input('sortBy', 'newest');
        switch ($sortBy)
        {
            case 'newest':
                $posts = $posts->sortByDesc('date');
                break;
            case 'oldest':
                $posts = $posts->sortBy('date');
                break;
            case 'most-likes':
                $posts = $posts->sortByDesc(function ($post) {
                    return $post->likes->count();
                });
                break;
            case 'least-likes':
                $posts = $posts->sortBy(function ($post) {
                    return $post->likes->count();
                });
                break;
            default:
                break;
        }

        return view('groupWall', compact('user', 'posts', 'groupId'));
    }

    public function showPost(Request $request, $postId)
    {
        $user = Auth::user();
        $users = User::where('userId', '!=', $user->userId)->get();

        if ($postId)
        {
            $post = Post::findOrFail($postId);
        }
        else
        {
            $post = null;
        }
        return view('post-detail', compact('user', 'post', 'users'));
    }
}
