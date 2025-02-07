<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::create('tagsInPost', function (Blueprint $table) {
            $table->id('tagsInPostId');
            $table->foreignId('tagId')->constrained('tags', 'tagId')->onDelete('cascade');
            $table->foreignId('postId')->constrained('posts', 'postId')->onDelete('cascade');
            $table->timestamps();
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('tagsInPost');
    }
};
