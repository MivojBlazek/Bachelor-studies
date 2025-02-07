<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Notifications\Notifiable;
use Laravel\Sanctum\HasApiTokens;
use App\Enums\RefereeRole;

class Control extends Model
{
    /** @use HasFactory<\Database\Factories\ControlFactory> */
    use HasApiTokens, HasFactory, Notifiable;

    protected $table = 'controls';

    protected $fillable = [
        'refereeRole',
        'feedback',
        'game_id',
        'referee_id',
    ];

    protected $casts = [
        'refereeRole' => RefereeRole::class,
    ];

    public function game()
    {
        return $this->belongsTo(Game::class, 'game_id');
    }

    public function referee()
    {
        return $this->belongsTo(Referee::class);
    }

    public function payment()
    {
        return $this->hasOne(Payment::class);
    }

    public function videos()
    {
        return $this->hasMany(Video::class, 'control_id');
    }
}
