<?php

namespace App\Http\Controllers;

use App\Http\Requests\LoginRequest;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;

class AuthController extends Controller
{
    public function login(LoginRequest $request)
    {
        $data = $request->validated();
        
        if (!Auth::attempt($data))
        {
            return response()->json([
                'errors' => [
                    'credentials' => ['Invalid credentials!']
                ]
            ], 422);
        }
        $user = Auth::user();
        $token = $user->createToken('main')->plainTextToken;

        $role = null;
        if ($user instanceof \App\Models\Delegate)
        {
            $role = 'delegate';
        }
        elseif ($user instanceof \App\Models\Referee)
        {
            $role = 'referee';
        }
        elseif ($user instanceof \App\Models\Club)
        {
            $role = 'club';
        }

        return response()->json([
            'user' => $user,
            'token' => $token,
            'role' => $role,
        ]);

    }

    public function logout(Request $request)
    {
        $user = $request->user();

        $user->currentAccessToken()->delete();

        return response()->noContent();
    }
}