<?php
/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

namespace App\Http\Controllers\referee;

use Illuminate\Http\Request;
use App\Models\Game;
use App\Models\Control;
use App\Http\Controllers\Controller;

class AllMatchesController extends Controller
{
    public function getAllGames()
    {
        $allGames = Game::with([
            'club1' => function ($query) {
                $query->select('id', 'name');
            },
            'club2' => function ($query) {
                $query->select('id', 'name');
            },
            'controls' => function ($query) {
                $query->select('game_id', 'referee_id', 'refereeRole')
                      ->with('referee:id,name');
            }
        ])
        ->orderBy('date', 'asc')
        ->get();

        if ($allGames->isEmpty()) {
            return response()->json(['message' => 'No games found'], 404);
        }

        return response()->json($allGames);
    }
}
