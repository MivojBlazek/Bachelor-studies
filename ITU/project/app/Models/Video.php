<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Notifications\Notifiable;
use Laravel\Sanctum\HasApiTokens;

class Video extends Model
{
    /** @use HasFactory<\Database\Factories\VideoFactory> */
    use HasApiTokens, HasFactory, Notifiable;

    protected $table = 'videos';

    protected $fillable = [
        'url',
        'description',
        'uploadedAt',
        'game_id',
        'uploaded_by',
    ];

    public function game()
    {
        return $this->belongsTo(Game::class, 'game_id');
    }

    public function delegate()
    {
        return $this->belongsTo(Delegate::class, 'uploaded_by');
    }
}
