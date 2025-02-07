<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use App\Models\User;
use App\Models\Post;

class Like extends Model
{
    /** @use HasFactory<\Database\Factories\LikeFactory> */
    use HasFactory;

    protected $table = 'likes';

    protected $primaryKey = 'likeId';

    protected $fillable = [
        'userId',
        'postId',
    ];

    public function user()
    {
        return $this->belongsTo(User::class, 'userId');
    }

    public function post()
    {
        return $this->belongsTo(Post::class, 'postId');
    }
}
