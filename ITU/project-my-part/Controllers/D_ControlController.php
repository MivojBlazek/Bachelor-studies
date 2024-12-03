<?php
/**
 * ITU project
 * 
 * File: D_ControlController.php
 * 
 * Author: Michal Blažek <xblaze38>
 */

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

        // Find control according to ID
        $control = Control::find($data['controlId']);
        if (!$control)
        {
            return response()->json(['error' => 'Control not found.'], 404);
        }

        // Update feedback and save
        $control->feedback = $data['feedback'];
        $control->save();
        return response()->json(['success' => 'Feedback updated successfully.'], 200);
    }
}
