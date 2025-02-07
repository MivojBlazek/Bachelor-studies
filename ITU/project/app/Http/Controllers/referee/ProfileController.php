<?php
/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

namespace App\Http\Controllers\referee;

use Illuminate\Http\Request;
use App\Models\Referee;
use Illuminate\Support\Facades\Auth;
use App\Http\Controllers\Controller;

class ProfileController extends Controller
{
    // Získání informací o rozhodčím
    public function show(Request $request)
    {
        $referee = Auth::user();

        if ($referee) {
            return response()->json($referee);
        }

        return response()->json(['error' => 'Referee not found'], 404);
    }
    
    // Aktualizace informací o uživateli
    public function update(Request $request)
    {
        
        $referee = Auth::user();

        if (!$referee) {
            return response()->json(['error' => 'Referee not found'], 404);
        }
        
        $request->validate([
            'name' => 'sometimes|required|string|max:255',
            'birth_date' => 'sometimes|required|date',
            'address' => 'sometimes|required|string|max:255',
            'phone' => 'sometimes|nullable|string|max:20',
            'email' => 'sometimes|nullable|email',
            'bank_account' => 'sometimes|required|string|max:50',
            'license' => 'sometimes|nullable|string|max:50',        ]);

        $referee->update($request->only([
            'name', 
            'birth_date',
            'address', 
            'phone', 
            'email', 
            'bank_account', 
            'license'
        ]));

        return response()->json(['message' => 'Data byla úspěšně uložena']);
    }
}