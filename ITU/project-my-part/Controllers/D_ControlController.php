<?php

namespace App\Http\Controllers\delegate;

use App\Http\Controllers\Controller;
use App\Http\Requests\delegate\FeedbackRequest;
use App\Models\Control;
use Illuminate\Http\Request;

class D_ControlController extends Controller
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
