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
        Schema::create('userInGroup', function (Blueprint $table) {
            $table->id('userInGroupId');
            $table->foreignId('userId')->constrained('users', 'userId')->onDelete('cascade');
            $table->foreignId('groupId')->constrained('groups', 'groupId')->onDelete('cascade');
            $table->string('approved');
            $table->timestamps();
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('userInGroup');
    }
};
