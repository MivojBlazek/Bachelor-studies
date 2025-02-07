<?php
/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

namespace App\Http\Controllers\referee;

use App\Models\Game;
use App\Http\Controllers\Controller;

class GameDetailController extends Controller
{
    public function getGameDetail($id)
    {
        $game = Game::with(['club1.players', 'club2.players', 'delegate', 'controls.referee', 'videos'])->find($id);

        if (!$game) {
            return response()->json(['message' => 'Hra nebyla nalezena.'], 404);
        }

        return response()->json($game);
    }
}