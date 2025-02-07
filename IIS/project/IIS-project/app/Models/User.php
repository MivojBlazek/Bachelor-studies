<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Foundation\Auth\User as Authenticatable;
use Illuminate\Notifications\Notifiable;
use App\Models\Group;
use App\Models\Post;
use App\Models\Wall;
use App\Models\Like;
use App\Models\Comment;

class User extends Authenticatable
{
    use HasFactory, Notifiable;

    protected $table = 'users';

    protected $primaryKey = 'userId';

    protected $fillable = [
        'name',
        'username',
        'email',
        'password',
        'age',
        'rights',
        'profile_photo',
        'banned_until',
        'bio',
    ];

    protected $hidden = [
        'password',
        'remember_token',
    ];

    protected $casts = [
        'password' => 'hashed',
        'banned_until' => 'datetime',
    ];

    public function adminGroups()
    {
        return $this->hasMany(Group::class, 'adminUserId');
    }

    public function memberGroups()
    {
        return $this->belongsToMany(Group::class, 'userInGroup', 'userId', 'groupId');
    }

    public function posts()
    {
        return $this->hasMany(Post::class, 'postCreatorId');
    }

    public function visiblePosts()
    {
        return $this->belongsToMany(Post::class, 'postVisibility', 'userId', 'postId');
    }

    public function wall()
    {
        return $this->hasOne(Wall::class, 'userId');
    }
    
    public function likes()
    {
        return $this->hasMany(Like::class, 'userId');
    }

    public function comments()
    {
        return $this->hasMany(Comment::class, 'userId');
    }
    
    // User banning
    public function isBanned()
    {
        return $this->banned_until && now()->lessThan($this->banned_until);
    }

    public function banFor($durationInMinutes)
    {
        $this->banned_until = now()->addMinutes($durationInMinutes);
        $this->save();
    }

    public function unban()
    {
        $this->banned_until = null;
        $this->save();
    }
}
