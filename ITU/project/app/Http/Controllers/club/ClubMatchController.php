<?php
/**
 * ITU project
 * 
 * Author: Matyáš Sapík <xsapik02>
 */

namespace App\Http\Controllers\club;

use App\Http\Requests\club\MatchRequest;
use Illuminate\Support\Facades\Auth;
use App\Http\Controllers\Controller;
use App\Models\Game; // Import modelu Game

class ClubMatchController extends Controller
{
    /**
     * Display the specified resource (Club match details).
     *
     * @param int $id
     * @return \Illuminate\Http\JsonResponse
     */
    public function show()
    {
        // Získání přihlášeného klubu
        $club = Auth::user();

        if (!$club) {
            return response()->json(['message' => 'Unauthorized'], 401);
        }

        // Získání všech zápasů, kde je klub buď domácí, nebo hostující
        $games = Game::with(['club1', 'club2', 'delegate', 'controls.referee'])
                     ->where('clubHome_id', $club->id)
                     ->orWhere('clubVisitor_id', $club->id)
                     ->get();

        if ($games->isEmpty()) {
            return response()->json(['message' => 'No matches found for your club'], 404);
        }

        return response()->json(['success' => true, 'data' => $games]);
    }

    public function update(MatchRequest $request, $id)
    {
        // Získání přihlášeného klubu
        $club = Auth::user();

        if (!$club) {
            return response()->json(['message' => 'Unauthorized'], 401);
        }

        // Najít zápas podle ID
        $game = Game::find($id);

        if (!$game) {
            return response()->json(['message' => 'Game not found'], 404);
        }

        // Zkontrolovat, zda klub má oprávnění upravit tento zápas
        if ($game->clubHome_id !== $club->id && $game->clubVisitor_id !== $club->id) {
            return response()->json(['message' => 'You do not have permission to update this game'], 403);
        }

        // Aktualizace dat zápasu (validovaná data jsou automaticky k dispozici)
        $game->update($request->validated());

        return response()->json([
            'success' => true,
            'data' => $game
        ]);
    }

    /**
     * Create a new game resource.
     *
     * @param \App\Http\Requests\club\MatchRequest $request
     * @return \Illuminate\Http\JsonResponse
     */
    public function create(MatchRequest $request)
    {
        // Získání přihlášeného klubu
        $club = Auth::user();

        if (!$club) {
            return response()->json(['message' => 'Unauthorized'], 401);
        }

        // Vytvoření nového zápasu
        $game = Game::create([
            'date' => $request->input('date'),
            'time' => $request->input('time'),
            'location' => $request->input('location'),
            'league' => $request->input('league'),
            'clubHome_id' => $club->id, // Přiřadí přihlášený klub jako domácí tým
            'clubVisitor_id' => $request->input('clubVisitor_id'),
        ]);

        return response()->json([
            'success' => true,
            'data' => $game
        ], 201);
    }

    /**
     * Remove the specified game resource.
     *
     * @param int $id
     * @return \Illuminate\Http\JsonResponse
     */
    public function destroy($id)
    {
        // Získání přihlášeného klubu
        $club = Auth::user();

        if (!$club) {
            return response()->json(['message' => 'Unauthorized'], 401);
        }

        // Najít zápas podle ID
        $game = Game::find($id);

        if (!$game) {
            return response()->json(['message' => 'Game not found'], 404);
        }

        // Kontrola oprávnění: Klub musí být domácím týmem
        if ($game->clubHome_id !== $club->id) {
            return response()->json(['message' => 'You do not have permission to delete this game'], 403);
        }

        // Smazání zápasu
        $game->delete();

        return response()->json([
            'success' => true,
            'message' => 'Game deleted successfully'
        ]);
    }

}

