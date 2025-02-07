<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Foundation\Auth\User as Authenticatable;
use Illuminate\Notifications\Notifiable;
use Laravel\Sanctum\HasApiTokens;

class Club extends Authenticatable
{
    /** @use HasFactory<\Database\Factories\DelegateFactory> */
    use HasApiTokens, HasFactory, Notifiable;

    protected $table = 'clubs';

    protected $fillable = [
        'name',
        'username',
        'password',
        'city',
        'address',
        'bank_account',
        'photo',
        'phone',
        'email',
    ];

    protected $hidden = [
        'password',
        'remember_token',
    ];

    protected $casts = [
        'password' => 'hashed',
    ];

    public function gamesAsHomeClub()
    {
        return $this->hasMany(Game::class, 'clubHome_id');
    }

    public function gamesAsVisitingClub()
    {
        return $this->hasMany(Game::class, 'clubVisitor_id');
    }

    public function players()
    {
        return $this->hasMany(Player::class, 'club_id');
    }
}
