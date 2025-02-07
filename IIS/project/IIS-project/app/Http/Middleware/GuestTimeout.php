<?php

namespace App\Http\Middleware;

use Closure;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\Log;

class GuestTimeout
{
    public function handle($request, Closure $next)
    {
        $user = Auth::user();

        if ($user && $user->rights === 'guest' && $user->created_at->diffInMinutes(now()) > 1)
        {
            $user->delete();
            Auth::logout();
            return redirect('/');
        }

        return $next($request);
    }
}
