<?php
/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

namespace App\Http\Controllers\referee;

use Illuminate\Http\Request;
use App\Models\Game;
use Illuminate\Support\Facades\Auth;
use App\Http\Controllers\Controller;

class PlayedMatchesController extends Controller
{
    public function getPlayedGames()
    {
        $referee = Auth::user();

        if (!$referee) {
            return response()->json(['error' => 'Referee not found'], 404);
        }

        $playedMatches = $referee->controls()
            ->whereHas('game', function ($query) {
                $query->where('date', '<', today());
            })
            ->with(['game' => function($query) {
                $query->orderBy('date', 'asc');
                },
                'game.club1' => function ($query) {
                    $query->select('id', 'name');
                },
                'game.club2' => function ($query) {
                    $query->select('id', 'name');
                },
                'game.delegate' => function ($query) {
                    $query->select('id', 'name', 'email', 'phone', 'address', 'photo');
                },
            ])
            ->get();

        if ($playedMatches->isEmpty()) {
            return response()->json(['message' => 'No played matches found'], 404);
        }

        return response()->json($playedMatches);
    }
}
