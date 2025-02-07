<?php
/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

namespace App\Http\Controllers\referee;

use Illuminate\Support\Facades\Auth;
use App\Http\Controllers\Controller;

class RegisteredMatchesController extends Controller
{
    public function getRegisteredGames()
    {
        $referee = Auth::user();

        if (!$referee) {
            return response()->json(['error' => 'Referee not found'], 404);
        }

        $registeredMatches = $referee->controls()
            ->whereHas('game', function ($query) {
                $query->where('date', '>=', today());
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
            ])
            ->get();

        if ($registeredMatches->isEmpty()) {
            return response()->json(['message' => 'No played matches found'], 404);
        }

        return response()->json($registeredMatches);
    }
}
