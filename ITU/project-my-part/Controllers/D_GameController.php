<?php
/**
 * ITU project
 * 
 * File: D_GameController.php
 * 
 * Author: Michal Blažek <xblaze38>
 */

namespace App\Http\Controllers\delegate;

use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use App\Models\Game;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\Log;
use Carbon\Carbon;


class D_GameController extends Controller
{
    public function games(Request $request)
    {
        $query = Game::with(['club1', 'club2', 'delegate', 'controls', 'videos']);

        // Filters
        if ($request->has('dateFrom') && !empty($request->input('dateFrom')))
        {
            $query->where('date', '>=', Carbon::parse($request->input('dateFrom')));
        }
        if ($request->has('dateTo') && !empty($request->input('dateTo')))
        {   
            $query->where('date', '<=', Carbon::parse($request->input('dateTo')));
        }
        if ($request->has('location') && !empty($request->input('location')))
        {
            $query->where('location', 'like', '%' . $request->input('location') . '%');
        }
        if ($request->has('league') && !empty($request->input('league')))
        {
            $query->where('league', 'like', '%' . $request->input('league') . '%');
        }

        $games = $query->get();
        return response()->json($games);
    }

    public function upcomingGames(Request $request)
    {
        $delegateId = $request->user()->id;

        $games = Game::with(['club1', 'club2', 'delegate', 'controls', 'videos'])
                     ->where('delegate_id', $delegateId)
                     ->where('date', '>', now())
                     ->get();
        return response()->json($games);
    }

    public function feedbackGames(Request $request)
    {
        $delegateId = $request->user()->id;

        // Find games in past with empty feedback
        $games = Game::with(['club1', 'club2', 'delegate', 'controls.referee', 'videos'])
                     ->where('delegate_id', $delegateId)
                     ->where('date', '<', now())
                     ->whereHas('controls', function ($query) {
                         $query->whereNull('feedback')
                               ->orWhere('feedback', '');
                     })
                     ->get();
        
        foreach ($games as $game)
        {
            $refereesWithoutFeedback = $game->controls->filter(function ($control) {
                return is_null($control->feedback) || trim($control->feedback) === '';
            });

            $game->refereesWithoutFeedback = $refereesWithoutFeedback->map(function ($control) {
                return $control->referee->name;
            })->values();
        }

        return response()->json($games);
    }

    public function show($id)
    {
        // Find game according to ID
        $game = Game::with(['club1', 'club2', 'delegate', 'controls.referee', 'videos'])->find($id);
        if (!$game)
        {
            return response()->json(['error' => 'Game not found.'], 404);
        }

        return response()->json($game);
    }

    public function signInToGame($id)
    {
        $delegate = Auth::user();

        // Find game
        $game = Game::find($id);
        if (!$game)
        {
            return response()->json(['error' => 'Game not found.'], 404);
        }

        // Save current user as delegate to the found game
        $game->delegate_id = $delegate->id;
        $game->save();
        return response()->json(['success' => 'Signed in to game successfully.'], 200);
    }

    public function signOutOfGame($id)
    {
        $game = Game::find($id);
        if (!$game)
        {
            return response()->json(['error' => 'Game not found.'], 404);
        }

        // Remove current user from found game
        $game->delegate_id = null;
        $game->save();
        return response()->json(['success' => 'Signed out from game successfully.'], 200);
    }
}
