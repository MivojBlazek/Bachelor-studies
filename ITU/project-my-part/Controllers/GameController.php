<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Models\Game;
use Illuminate\Support\Facades\Log;


class GameController extends Controller
{
    public function games(Request $request)
    {
        $query = Game::with(['club1', 'club2', 'delegate', 'controls', 'videos']);

        // Filters
        if ($request->has('dateFrom'))
        {
            $query->where('date', '>=', $request->input('dateFrom')); //TODO server error fix
        }
        if ($request->has('dateTo'))
        {   
            $query->where('date', '<=', $request->input('dateTo')); //TODO server error fix
        }
        if ($request->has('location'))
        {
            $query->where('location', 'like', '%' . $request->input('location') . '%');
        }
        if ($request->has('league'))
        {
            $query->where('league', 'like', '%' . $request->input('league') . '%');
        }

        // Sorts
        if ($request->has('sort'))
        {
            $sort = $request->input('sort');
            $query->orderBy($sort, 'asc');
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
        $game = Game::with(['club1', 'club2', 'delegate', 'controls.referee', 'videos'])->find($id);
        if (!$game)
        {
            return response()->json(['message' => 'Game not found'], 404);
        }

        return response()->json($game);
    }
}
