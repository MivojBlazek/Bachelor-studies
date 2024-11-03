<?php

namespace App\Http\Controllers;

use App\Http\Requests\FeedbackRequest;
use App\Models\Control;
use Illuminate\Http\Request;

class ControlController extends Controller
{
    public function updateFeedback(FeedbackRequest $request)
    {
        $data = $request->validated();

        $control = Control::find($data['controlId']);
        if (!$control)
        {
            return response()->json(['message' => 'Control not found'], 404);
        }

        $control->feedback = $data['feedback'];
        $control->save();
    }
}
