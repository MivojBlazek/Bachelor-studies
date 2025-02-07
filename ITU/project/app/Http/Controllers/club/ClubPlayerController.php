<?php
/**
 * ITU project
 * 
 * Author: Matyáš Sapík <xsapik02>
 */

namespace App\Http\Controllers\club;

use App\Http\Requests\club\PlayerRequest;
use App\Models\Player;
use Illuminate\Support\Facades\Auth;
use App\Http\Controllers\Controller;

class ClubPlayerController extends Controller
{
    /**
     * Display the authenticated club's players.
     */
    public function show()
    {
        $club = Auth::user();

        if (!$club) {
            return response()->json(['error' => 'Club not found'], 404);
        }

        $players = $club->players()->get();

        return response()->json([
            'club' => $club->only(['id', 'name', 'email']),
            'players' => $players,
        ]);
    }

    /**
     * Update the specified player associated with the authenticated club.
     */
    public function update(PlayerRequest $request, $id)
    {
        $club = Auth::user();

        if (!$club) {
            return response()->json(['error' => 'Club not found'], 404);
        }

        $player = Player::where('id', $id)
            ->where('club_id', $club->id)
            ->first();

        if (!$player) {
            return response()->json(['error' => 'Player not found or does not belong to the club'], 404);
        }

        $data = $request->validated();

        $player->update($data);

        return response()->json([
            'message' => 'Player updated successfully',
            'player' => $player,
        ], 200);
    }

    /**
     * Create a new player for the authenticated club.
     */
    public function create(PlayerRequest $request)
    {
        $club = Auth::user();

        if (!$club) {
            return response()->json(['error' => 'Club not found'], 404);
        }

        $data = $request->validated();

        $existingPlayer = Player::where('club_id', $club->id)
            ->where(function ($query) use ($data) {
                $query->where('email', $data['email'])
                      ->orWhere('dress_number', $data['dress_number']);
            })->first();

        if ($existingPlayer) {
            return response()->json([
                'error' => 'A player with this email or dress number already exists in your club.',
            ], 422);
        }

        $data['club_id'] = $club->id;

        try {
            $player = Player::create($data);

            return response()->json([
                'message' => 'Player created successfully',
                'player' => $player,
            ], 201);
        } catch (\Exception $e) {
            return response()->json([
                'error' => 'An error occurred while creating the player.',
                'details' => $e->getMessage(),
            ], 500);
        }
    }

    /**
     * Remove the specified player from the authenticated club.
     */
    public function delete($id)
    {
        $club = Auth::user();

        if (!$club) {
            return response()->json(['error' => 'Club not found'], 404);
        }

        $player = Player::where('id', $id)
            ->where('club_id', $club->id)
            ->first();

        if (!$player) {
            return response()->json(['error' => 'Player not found or does not belong to the club'], 404);
        }

        try {
            $player->delete();

            return response()->json([
                'message' => 'Player deleted successfully',
            ], 200);
        } catch (\Exception $e) {
            return response()->json([
                'error' => 'An error occurred while deleting the player.',
                'details' => $e->getMessage(),
            ], 500);
        }
    }
}
