<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use App\Models\Post;

class Tag extends Model
{
    /** @use HasFactory<\Database\Factories\TagFactory> */
    use HasFactory;

    protected $table = 'tags';

    protected $primaryKey = 'tagId';

    protected $fillable = [
        'name',
    ];

    public function posts()
    {
        return $this->belongsToMany(Post::class, 'tagsInPost', 'tagId', 'postId');
    }
}
