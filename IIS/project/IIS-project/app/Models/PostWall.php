<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use App\Models\Wall;
use App\Models\Post;

class PostWall extends Model
{
    /** @use HasFactory<\Database\Factories\PostWallFactory> */
    use HasFactory;

    protected $table = 'postWall';

    protected $primaryKey = 'postWallId';

    protected $fillable = [
        'wallId',
        'postId',
    ];

    public function wall()
    {
        return $this->belongsTo(Wall::class, 'wallId');
    }

    public function post()
    {
        return $this->belongsTo(Post::class, 'postId');
    }
}
