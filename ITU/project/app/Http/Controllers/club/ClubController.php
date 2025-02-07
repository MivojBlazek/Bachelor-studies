<?php
/**
 * ITU project
 * 
 * Author: Matyáš Sapík <xsapik02>
 */

namespace App\Http\Controllers\club;

use App\Http\Requests\club\UpdateClubRequest;
use Illuminate\Support\Facades\Auth;
use App\Http\Controllers\Controller;
use App\Models\Club;

class ClubController extends Controller
{

    /**
     * Display the specified resource.
     */
    public function show()
    {
        $club = Auth::user();
        // Return the single club data directly as JSON
        return response()->json($club);
    }

    /**
     * Update the specified resource in storage.
     */
    public function update(UpdateClubRequest $request)
    {
        $club = Auth::user();
        // Validate the incoming data from the request
        $data = $request->validated();

        // Update the club record with the validated data
        $club->update($data);

        // Return the updated club data as a JSON response
        return response()->json($club, 200);
    }

    public function index()
    {
        $clubs = Club::all(['id', 'name']); // Vrátí pouze ID a název klubu

        if ($clubs->isEmpty()) {
            return response()->json(['message' => 'No clubs found'], 404);
        }

        return response()->json(['success' => true, 'data' => $clubs]);
    }
}
