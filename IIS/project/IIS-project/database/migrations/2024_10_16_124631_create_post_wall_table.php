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
        Schema::create('postWall', function (Blueprint $table) {
            $table->id('postWallId');
            $table->foreignId('postId')->constrained('posts', 'postId')->onDelete('cascade');
            $table->foreignId('wallId')->constrained('walls', 'wallId')->onDelete('cascade');
            $table->timestamps();
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('postWall');
    }
};
