<?php

namespace App\Providers;

use Illuminate\Auth\EloquentUserProvider;
use Illuminate\Contracts\Auth\Authenticatable;
use App\Models\Delegate;
use App\Models\Referee;
use App\Models\Club;

class UserProvider extends EloquentUserProvider
{
    public function retrieveByCredentials(array $credentials)
    {
        $username = $credentials['username'];
        return Delegate::where('username', $username)->first() ??
               Referee::where('username', $username)->first() ??
               Club::where('username', $username)->first();
    }

    public function validateCredentials(Authenticatable $user, array $credentials)
    {
        return password_verify($credentials['password'], $user->getAuthPassword());
    }
}
