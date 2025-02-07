<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use App\Models\Comment;

class CommentController extends Controller
{
    public function showComments(Request $request)
    {
        $user = Auth::user();

        $request->validate([
            'postId' => 'required|exists:posts,postId',
        ]);

        $comments = Comment::with('user')->where('postId', $request->postId)->latest()->get();
        
        return response()->json($comments, 200);
    }

    public function addComment(Request $request)
    {
        $user = Auth::user();

        $request->validate([
            'postId' => 'required|exists:posts,postId',
            'content' => 'required|string',
        ]);

        Comment::create([
            'content' => $request->content,
            'userId' => $user->userId,
            'postId' => $request->postId,
        ]);
        return redirect()->back()->with('success', 'Comment created successfully.');
    }

    public function removeComment(Request $request)
    {
        $request->validate([
            'commentId' => 'required|exists:comments,commentId',
        ]);

        $existingComment = Comment::where('commentId', $request->commentId)->first();
        if ($existingComment)
        {
            $existingComment->delete();
        }
        return redirect()->back()->with('success', 'Comment removed successfully.');
    }
}
