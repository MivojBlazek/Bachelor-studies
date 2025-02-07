<?php
/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

namespace App\Http\Controllers\referee;

use App\Models\Control;
use App\Models\Game;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Validator;
use Illuminate\Support\Facades\Auth;
use App\Http\Controllers\Controller;

class AddControlController extends Controller
{
    public function addControl(Request $request)
    {
        $ref = Auth::user();
        
        if (!$ref) {
            return response()->json(['error' => 'Referee not found'], 404);
        }

        $validator = Validator::make($request->all(), [
            'game_id' => 'required|exists:games,id',
            'refereeRole' => 'required|in:main,line',
        ]);

        if ($validator->fails()) {
            return response()->json(['error' => $validator->errors()], 400);
        }

        $game = Game::find($request->game_id);

        $existingControl = Control::where('game_id', $game->id)
            ->where('referee_id', $ref->id)
            ->whereIn('refereeRole', ['main', 'line'])
            ->first();

        if ($existingControl) {
            return response()->json(['message' => 'This referee is already assigned to the game in this role.'], 400);
        }

        if ($request->refereeRole === 'line') {
            $lineReferees = Control::where('game_id', $game->id)
                ->where('refereeRole', 'line')
                ->count();

            if ($lineReferees >= 2) {
                return response()->json(['message' => 'Both line referees are already assigned to this game.'], 400);
            }
        }

        $control = Control::create([
            'game_id' => $game->id,
            'referee_id' => $ref->id,
            'refereeRole' => $request->refereeRole,
        ]);

        
    
        return response()->json(['game_id' => $control->game_id, 
        'referee_id' => $control->referee_id,
        'refereeRole' => $control->refereeRole,
        'referee' => [
            'id' => $control->referee->id,
            'name' => $control->referee->name,
        ],
        ]);
    
    }
}
