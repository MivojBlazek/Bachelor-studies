<?php
/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

namespace App\Http\Controllers\referee;

use App\Models\Control;
use Illuminate\Http\Request;
use App\Http\Controllers\Controller;
use Illuminate\Support\Facades\Auth;

class GameControlController extends Controller
{
    public function deleteControl(Request $request)
    {
        $gameId = $request->input('game_id');
        $referee = Auth::user();

        if (!$referee) {
            return response()->json(['error' => 'Referee not found'], 404);
        }

        $control = Control::where('game_id', $gameId)
                        ->where('referee_id', $referee->id)
                        ->first();

        if (!$control) {
            return response()->json(['message' => 'Control not found'], 404);
        }

        $control->delete();

        return response()->json(['message' => 'Control deleted successfully']);
    }
}