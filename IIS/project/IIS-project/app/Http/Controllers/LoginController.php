<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\Hash;
use App\Models\User;

class LoginController extends Controller
{
    public function showLoginForm()
    {
        if (Auth::check())
        {
            return redirect()->route('FYP');
        }
        return view('login');
    }

    public function login(Request $request)
    {
        $request->validate([
            'username' => 'required|string',
            'password' => 'required',
        ]);

        if (Auth::attempt($request->only('username', 'password')))
        {
            $user = Auth::user();
            if ($user->isBanned())
            {
                Auth::logout();
                return redirect()->route('login')->with('error', 'You are banned. Try again later.');
            }
            return redirect()->route('FYP');
        }

        return back()->withErrors([
            'username' => 'Incorrect credentials.',
        ]);
    }

    public function guestLogin()
    {
        $guestId = uniqid();
        $guest = User::create([
            'name' => 'Guest',
            'username' => 'guest_' . $guestId,
            'email' => 'guest_' . $guestId . '@example.com',
            'password' => bcrypt('guest'),
            'rights' => 'guest',
            'profile_photo' => 'https://cdn.pixabay.com/photo/2015/10/05/22/37/blank-profile-picture-973460_1280.png',
        ]);

        Auth::login($guest);
        return redirect()->route('FYP');
    }
    
    public function logout()
    {
        Auth::logout();
        return redirect()->route('login');
    }
}
