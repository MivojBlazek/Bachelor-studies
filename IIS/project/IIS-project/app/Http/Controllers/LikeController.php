<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use App\Models\Like;

class LikeController extends Controller
{
    public function toggleLike(Request $request)
    {
        $user = Auth::user();

        $request->validate([
            'postId' => 'required|exists:posts,postId',
        ]);

        $existingLike = Like::where('userId', $user->userId)->where('postId', $request->postId)->first();
        if ($existingLike)
        {
            // Remove
            $existingLike->delete();
            return redirect()->back()->with('success', 'Like removed successfully.');
        }
        else
        {
            Like::create([
                'userId' => $user->userId,
                'postId' => $request->postId,
            ]);
            return redirect()->back()->with('success', 'Like added successfully.');
        }
    }
}
