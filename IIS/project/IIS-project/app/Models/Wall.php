<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use App\Models\Post;
use App\Models\User;
use App\Models\Group;

class Wall extends Model
{
    /** @use HasFactory<\Database\Factories\WallFactory> */
    use HasFactory;

    protected $table = 'walls';

    protected $primaryKey = 'wallId';

    protected $fillable = [
        'userId',
        'groupId',
    ];

    public function posts()
    {
        return $this->belongsToMany(Post::class, 'postWall', 'wallId', 'postId');
    }

    public function user()
    {
        return $this->belongsTo(User::class, 'userId');
    }

    public function group()
    {
        return $this->belongsTo(Group::class, 'groupId');
    }
}
