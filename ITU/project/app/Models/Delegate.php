<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Foundation\Auth\User as Authenticatable;
use Illuminate\Notifications\Notifiable;
use Laravel\Sanctum\HasApiTokens;

class Delegate extends Authenticatable
{
    /** @use HasFactory<\Database\Factories\DelegateFactory> */
    use HasApiTokens, HasFactory, Notifiable;

    protected $table = 'delegates';

    protected $fillable = [
        'name',
        'username',
        'password',
        'license',
        'address',
        'bank_account',
        'photo',
        'birth_date',
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

    public function games()
    {
        return $this->hasMany(Game::class);
    }

    public function controls()
    {
        return $this->hasMany(Control::class);
    }

    public function payments()
    {
        return $this->hasMany(Payment::class, 'approved_by');
    }

    public function videos()
    {
        return $this->hasMany(Video::class, 'uploaded_by');
    }
}
