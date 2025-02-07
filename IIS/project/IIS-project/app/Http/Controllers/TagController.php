<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use App\Models\Tag;

class TagController extends Controller
{
    public function removeTag(Request $request)
    {
        $request->validate([
            'tagId' => 'required|exists:tags,tagId',
        ]);

        $existingTag = Tag::where('tagId', $request->tagId)->first();
        if ($existingTag)
        {
            $existingTag->delete();
        }
        return redirect()->back()->with('success', 'Tag removed successfully.');
    }

    public function showPosts(Request $request)
    {
        $request->validate([
            'name' => 'required|exists:tags,name',
        ]);

        $tag = Tag::with('posts')->where('name', $request->name)->first();
        if ($tag)
        {
            return response()->json(['posts' => $tag->posts], 200);
        } 
        return response()->json(['posts' => []], 200);
    }
}
