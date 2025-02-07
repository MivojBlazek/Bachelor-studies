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
        Schema::create('likes', function (Blueprint $table) {
            $table->id('likeId');
            $table->foreignId('userId')->constrained('users', 'userId')->onDelete('cascade');
            $table->foreignId('postId')->constrained('posts', 'postId')->onDelete('cascade');
            $table->timestamps();

            // User can like the post only once
            $table->unique(['userId', 'postId']);
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('likes');
    }
};
