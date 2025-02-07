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
use App\Models\Payment;
use Illuminate\Support\Facades\Auth;
use App\Http\Controllers\Controller;

class PaymentsController extends Controller
{
    public function getPayment()
    {
        $referee = Auth::user();

        if (!$referee) {
            return response()->json(['error' => 'Referee not found'], 404);
        }

        $playedMatches = $referee->controls()
            ->whereHas('game', function ($query) {
                $query->where('date', '<', today());
            })
            ->with([
                'game' => function($query) {
                    $query->orderBy('date', 'asc');
                },
                'game.club1' => function ($query) {
                    $query->select('id', 'name');
                },
                'game.club2' => function ($query) {
                    $query->select('id', 'name');
                },
                'payment' => function ($query) {
                    $query->select('id', 'amount', 'approvalDate', 'control_id', 'approved_by')
                          ->with('delegate');
                }
            ])
            ->get();

        if ($playedMatches->isEmpty()) {
            return response()->json(['message' => 'No played matches found'], 404);
        }

        $result = $playedMatches->map(function($match) {
            return [
                'controlId' => $match->id,
                'game' => [
                    'id' => $match->game->id,
                    'date' => $match->game->date,
                    'club1' => $match->game->club1->name,
                    'club2' => $match->game->club2->name,
                    'time' => $match->game->time,
                    'location' => $match->game->location,
                    'league' => $match->game->league,
                ],
                'refereeRole' => $match->refereeRole,
                'payments' => $match->payment ? [
                    'amount' => $match->payment->amount,
                    'approvalDate' => $match->payment->approvalDate,
                    'approvedBy' => $match->payment->delegate ? $match->payment->delegate->name : 'N/A',
                ] : [],
            ];
        });
        return response()->json($result);
    }
}
