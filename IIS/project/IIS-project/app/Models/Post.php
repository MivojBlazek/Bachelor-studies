<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use App\Models\User;
use App\Models\Wall;
use App\Models\Like;
use App\Models\Comment;
use App\Models\Tag;

class Post extends Model
{
    /** @use HasFactory<\Database\Factories\PostFactory> */
    use HasFactory;

    protected $table = 'posts';

    protected $primaryKey = 'postId';

    protected $fillable = [
        'photo',
        'description',
        'place',
        'date',
        'postCreatorId',
        'visibility',
    ];

    public function creator()
    {
        return $this->belongsTo(User::class, 'postCreatorId');
    }

    public function visibleForUsers()
    {
        return $this->belongsToMany(User::class, 'postVisibility', 'postId', 'userId');
    }

    public function walls()
    {
        return $this->belongsToMany(Wall::class, 'postWall', 'postId', 'wallId');
    }
    
    public function likes()
    {
        return $this->hasMany(Like::class, 'postId');
    }

    public function comments()
    {
        return $this->hasMany(Comment::class, 'postId');
    }

    public function tags()
    {
        return $this->belongsToMany(Tag::class, 'tagsInPost', 'postId', 'tagId');
    }
}
