<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\Hash;
use App\Models\User;

class RegisterController extends Controller
{
    public function showRegistrationForm()
    {
        return view('register');
    }

    public function register(Request $request)
    {
        $request->validate([
            'name' => 'required|string|max:255',
            'username' => 'required|string|max:255|unique:users',
            'email' => 'required|string|email|max:255',
            'password' => 'required|string|confirmed',
            'age' => 'nullable|integer|min:1|max:150',
        ],
        [
            'username.unique' => 'The username has already been taken.',
            'password.confirmed' => 'The password and confirmation password do not match.',
        ]);

        User::create([
            'name' => $request->name,
            'username' => $request->username,
            'email' => $request->email,
            'password' => Hash::make($request->password),
            'age' => $request->age,
            'rights' => 'user',
            'banned_until' => null,
            'profile_photo' => 'https://plus.unsplash.com/premium_vector-1727955580230-5e2bafc43bba?q=80&w=2360&auto=format&fit=crop&ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D',
        ]);

        return redirect()->route('login')->with('success', 'Registration successful. You can now log in.');
    }
}
