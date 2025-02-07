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
        Schema::create('games', function (Blueprint $table) {
            $table->id();
            $table->date('date');
            $table->time('time');
            $table->string('location');
            $table->string('league');
            $table->foreignId('clubHome_id')->constrained('clubs')->onDelete('cascade');
            $table->foreignId('clubVisitor_id')->constrained('clubs')->onDelete('cascade');
            $table->foreignId('delegate_id')->nullable()->constrained('delegates')->onDelete('cascade');
            $table->timestamps();
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('games');
    }
};
