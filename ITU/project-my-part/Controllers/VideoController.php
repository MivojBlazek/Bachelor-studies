<?php

namespace App\Http\Controllers\delegate;

use App\Http\Controllers\Controller;
use App\Http\Requests\delegate\VideoRequest;
use App\Models\Video;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;


class VideoController extends Controller
{
    public function addVideo(VideoRequest $request)
    {
        $delegate = Auth::user();
        $data = $request->validated();

        Video::create([
            'url' => $data['url'],
            'description' => $data['description'],
            'uploadedAt' => now(),
            'game_id' => $data['id'],
            'uploaded_by' => $delegate->id,
        ]);
    }
}
