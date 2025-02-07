<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use App\Models\User;
use App\Models\Wall;

class Group extends Model
{
    /** @use HasFactory<\Database\Factories\GroupFactory> */
    use HasFactory;

    protected $table = 'groups';

    protected $primaryKey = 'groupId';

    protected $fillable = [
        'name',
        'adminUserId',
    ];

    public function userAsAdmin()
    {
        return $this->belongsTo(User::class, 'adminUserId');
    }

    public function users()
    {
        return $this->belongsToMany(User::class, 'userInGroup', 'groupId', 'userId');
    }

    public function wall()
    {
        return $this->hasOne(Wall::class, 'groupId');
    }
}
