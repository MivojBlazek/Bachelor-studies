<?php

use App\Http\Controllers\AuthController;
use App\Http\Controllers\GameController;
use App\Http\Controllers\ControlController;
use App\Http\Controllers\PaymentController;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;


/*
|--------------------------------------------------------------------------
| API Routes
|--------------------------------------------------------------------------
|
| Here is where you can register API routes for your application. These
| routes are loaded by the RouteServiceProvider and all of them will
| be assigned to the "api" middleware group. Make something great!
|
*/

Route::middleware('auth:sanctum')->group(function() {
    Route::get('/logout', [AuthController::class, 'logout']);

    Route::get('/user', function (Request $request) {
        return $request->user();
    });

    Route::get('/games', [GameController::class, 'games']);
    Route::get('/upcomingGames', [GameController::class, 'upcomingGames']);
    Route::get('/feedbackGames', [GameController::class, 'feedbackGames']);
    Route::get('/payments', [PaymentController::class, 'payments']);
    Route::get('/games/{id}', [GameController::class, 'show']);

    Route::post('/controls', [ControlController::class, 'updateFeedback']);
});

Route::post('/login', [AuthController::class, 'login']);
