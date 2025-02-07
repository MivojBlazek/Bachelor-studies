<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use App\Models\Post;
use App\Models\PostVisibility;
use App\Models\Tag;
use App\Models\User;

class PostController extends Controller
{
    public function updateDescription(Request $request)
    {
        $request->validate([
            'postId' => 'required|exists:posts,postId',
            'description' => 'string',
        ]);

        $existingPost = Post::where('postId', $request->postId)->first();
        if ($existingPost)
        {
            // Remove all tags from post
            $existingPost->tags()->detach();

            $existingPost->description = $request->description;
            $existingPost->save();

            //Add tags to post
            $this->fetchTagsFromDescription($existingPost, $request->description);
        }
        return redirect()->back()->with('success', 'Description updated successfully.');
    }

    public function showAddPost()
    {
        $user = Auth::user();
        $users = User::where('userId', '!=', $user->userId)->get();
        return view('addPost', compact('user', 'users'));
    }

    public function removePost(Request $request)
    {
        $request->validate([
            'postId' => 'required|exists:posts,postId',
        ]);

        $existingPost = Post::where('postId', $request->postId)->first();
        if ($existingPost)
        {
            $existingPost->delete();
        }
        return redirect()->route('FYP')->with('success', 'Post removed successfully.');
    }

    public function addPost(Request $request)
    {
        $user = Auth::user();

        $request->validate([
            'photo' => 'required|string',
            'description' => 'nullable|string|max:255',
            'place' => 'nullable|string',
            'visibility' => 'required|string',
            'users' => 'nullable|array',
            'users.*' => 'exists:users,userId'
        ]);

        $post = Post::create([
            'photo' => $request->input('photo'),
            'description' => $request->input('description'),
            'place' => $request->input('place'),
            'date' => now(),
            'postCreatorId' => $user->userId,
            'visibility' => $request->visibility,
        ]);

        if ($request->has('users'))
        {
            // Visible to me
            PostVisibility::create([
                'postId' => $post->postId,
                'userId' => $user->userId,
            ]);

            foreach ($request->users as $userId)
            {
                PostVisibility::create([
                    'postId' => $post->postId,
                    'userId' => $userId,
                ]);
            }
        }

        $this->fetchTagsFromDescription($post, $request->description);

        return redirect()->route('profile', ['username' => $user->username])->with('success', 'Post created successfully.');
    }

    protected function fetchTagsFromDescription(Post $post, $description)
    {
        preg_match_all('/#([a-zA-Z0-9_-]+)/', $description, $matches);
        if (!empty($matches[1]))
        {
            foreach ($matches[1] as $tagName)
            {
                $tag = Tag::firstOrCreate(['name' => $tagName]);

                $post->tags()->attach($tag->tagId);
            }
        }
    }
}
