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

class DashboardController extends Controller
{
    public function getAssignedGames(Request $request)
    {
        $referee = Auth::user();

        if (!$referee) {
            return response()->json(['error' => 'Referee not found'], 404);
        }

        $assignedGames = $referee->controls()
            ->whereHas('game', function($query) {
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
        
        if ($assignedGames->isEmpty()) {
            return response()->json(['message' => 'No upcoming games found'], 404);
        }
        return response()->json($assignedGames);
    }
}