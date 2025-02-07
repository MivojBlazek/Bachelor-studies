<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use App\Models\Tag;
use App\Models\Post;

class TagsInPost extends Model
{
    /** @use HasFactory<\Database\Factories\TagsInPostFactory> */
    use HasFactory;

    protected $table = 'tagsInPost';

    protected $primaryKey = 'tagsInPostId';

    protected $fillable = [
        'tagId',
        'postId',
    ];

    public function tag()
    {
        return $this->belongsTo(Tag::class, 'tagId');
    }

    public function post()
    {
        return $this->belongsTo(Post::class, 'postId');
    }
}
