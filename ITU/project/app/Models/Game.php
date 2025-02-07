<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Notifications\Notifiable;
use Laravel\Sanctum\HasApiTokens;

class Game extends Model
{
    /** @use HasFactory<\Database\Factories\GameFactory> */
    use HasApiTokens, HasFactory, Notifiable;

    protected $table = 'games';

    protected $fillable = [
        'date',
        'time',
        'location',
        'clubHome_id',
        'clubVisitor_id',
        'delegate_id',
        'league',
    ];

    public function club1()
    {
        return $this->belongsTo(Club::class, 'clubHome_id');
    }

    public function club2()
    {
        return $this->belongsTo(Club::class, 'clubVisitor_id');
    }

    public function delegate()
    {
        return $this->belongsTo(Delegate::class);
    }

    public function controls()
    {
        return $this->hasMany(Control::class, 'game_id');
    }

    public function videos()
    {
        return $this->hasMany(Video::class, 'game_id');
    }
}
