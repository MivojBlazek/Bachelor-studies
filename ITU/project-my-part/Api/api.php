<?php

use App\Http\Controllers\AuthController;
use App\Http\Controllers\delegate\GameController;
use App\Http\Controllers\delegate\ControlController;
use App\Http\Controllers\delegate\PaymentController;
use App\Http\Controllers\delegate\ProfileController;
use App\Http\Controllers\delegate\VideoController;
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

    Route::get('/delegate/games', [GameController::class, 'games']);
    Route::get('/delegate/upcomingGames', [GameController::class, 'upcomingGames']);
    Route::get('/delegate/feedbackGames', [GameController::class, 'feedbackGames']);
    Route::get('/delegate/allPayments', [PaymentController::class, 'payments']);
    Route::get('/delegate/payments', [PaymentController::class, 'notApprovedPayments']);
    Route::get('/delegate/games/{id}', [GameController::class, 'show']);

    Route::get('/delegate/delegate_profile/{delegateId}', [ProfileController::class, 'getDelegate']);
    Route::get('/delegate/club_profile/{clubId}', [ProfileController::class, 'getClub']);
    Route::get('/delegate/referee_profile/{refereeId}', [ProfileController::class, 'getReferee']);

    Route::post('/delegate/controls', [ControlController::class, 'updateFeedback']);

    Route::post('/delegate/videos', [VideoController::class, 'addVideo']);

    Route::get('/delegate/payments/{paymentId}', [PaymentController::class, 'getPayment']);
    Route::post('/delegate/payments/{paymentId}/approve', [PaymentController::class, 'approvePayment']);
    Route::post('/delegate/payments/{paymentId}/decline', [PaymentController::class, 'declinePayment']);

    Route::post('/delegate/game/{id}/signUp', [GameController::class, 'signInToGame']);
    Route::post('/delegate/game/{id}/signOut', [GameController::class, 'signOutOfGame']);
});

Route::post('/login', [AuthController::class, 'login']);
