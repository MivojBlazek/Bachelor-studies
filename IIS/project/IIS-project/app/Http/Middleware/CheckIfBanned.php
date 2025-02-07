<?php

namespace App\Http\Middleware;

use Closure;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;

class CheckIfBanned
{
    public function handle(Request $request, Closure $next)
    {
        if (Auth::check() && Auth::user()->isBanned())
        {
            Auth::logout();
            return redirect()->route('login')->with('error', 'You are banned. Try again later.');
        }

        return $next($request);
    }
}
