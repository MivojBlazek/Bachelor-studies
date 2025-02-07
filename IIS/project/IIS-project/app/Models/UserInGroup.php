<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class UserInGroup extends Model
{
    use HasFactory;

    protected $table = 'userInGroup';

    protected $primaryKey = 'userInGroupId';

    protected $fillable = [
        'userId',
        'groupId',
        'approved',
    ];

    public function user()
    {
        return $this->belongsTo(User::class, 'userId');
    }

    public function group()
    {
        return $this->belongsTo(Group::class, 'groupId');
    }
}
