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
        Schema::create('walls', function (Blueprint $table) {
            $table->id('wallId');
            $table->foreignId('userId')->nullable()->constrained('users', 'userId')->onDelete('cascade');
            $table->foreignId('groupId')->nullable()->constrained('groups', 'groupId')->onDelete('cascade');
            $table->timestamps();

            // Only userId OR groupId
            $table->unique(['userId', 'groupId']);
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('walls');
    }
};
