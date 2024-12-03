<?php
/**
 * ITU project
 * 
 * File: D_ProfileController.php
 * 
 * Author: Michal Blažek <xblaze38>
 */

namespace App\Http\Controllers\delegate;

use App\Http\Controllers\Controller;
use App\Models\Delegate;
use App\Models\Club;
use App\Models\Referee;
use Illuminate\Http\Request;

class D_ProfileController extends Controller
{
    public function getDelegate($delegateId)
    {
        // Find delegate according to ID
        $delegate = Delegate::find($delegateId);
        if (!$delegate)
        {
            return response()->json(['error' => 'Delegate not found.'], 404);
        }

        return response()->json($delegate);
    }

    public function getClub($clubId)
    {
        // Find club according to ID
        $club = Club::with('players')->find($clubId);
        if (!$club)
        {
            return response()->json(['error' => 'Club not found.'], 404);
        }

        return response()->json($club);
    }

    public function getReferee($refereeId)
    {
        // Find referee according to ID
        $referee = Referee::find($refereeId);
        if (!$referee)
        {
            return response()->json(['error' => 'Referee not found.'], 404);
        }

        return response()->json($referee);
    }
}
