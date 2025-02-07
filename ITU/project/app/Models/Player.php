<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Player extends Model
{
    /** @use HasFactory<\Database\Factories\PlayerFactory> */
    use HasFactory;

    protected $table = 'players';

    protected $fillable = [
        'name',
        'dress_number',
        'role',
        'address',
        'phone',
        'email',
        'club_id',
    ];

    public function club()
    {
        return $this->belongsTo(Club::class, 'club_id');
    }
}
