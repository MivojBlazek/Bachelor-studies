<?php

namespace App\Http\Controllers\delegate;

use App\Http\Controllers\Controller;
use App\Models\Delegate;
use App\Models\Club;
use App\Models\Referee;
use Illuminate\Http\Request;

class ProfileController extends Controller
{
    public function getDelegate($delegateId)
    {
        $delegate = Delegate::find($delegateId);
        if (!$delegate)
        {
            return response()->json(['message' => 'Delegate not found'], 404);
        }

        return response()->json($delegate);
    }

    public function getClub($clubId)
    {
        $club = Club::with('players')->find($clubId);
        if (!$club)
        {
            return response()->json(['message' => 'Club not found'], 404);
        }

        return response()->json($club);
    }

    public function getReferee($refereeId)
    {
        $referee = Referee::find($refereeId);
        if (!$referee)
        {
            return response()->json(['message' => 'Referee not found'], 404);
        }

        return response()->json($referee);
    }
}
