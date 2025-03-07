<?php
/**
 * ITU project
 * 
 * File: api.php
 * 
 * Authors: Michal Blažek <xblaze38>
 *          Matěj Lepeška <xlepes00>
 *          Matyáš Sapík  <xsapik02>
 */

use App\Http\Controllers\AuthController;
use App\Http\Controllers\delegate\D_GameController;
use App\Http\Controllers\delegate\D_ControlController;
use App\Http\Controllers\delegate\D_PaymentController;
use App\Http\Controllers\delegate\D_ProfileController;
use App\Http\Controllers\delegate\D_VideoController;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;


Route::middleware('auth:sanctum')->group(function() {
    Route::get('/logout', [AuthController::class, 'logout']);

    Route::get('/user', function (Request $request) {
        return $request->user();
    });

    /** Delegate routes (Michal Blažek) */
    Route::get('/delegate/games', [D_GameController::class, 'games']);
    Route::get('/delegate/upcomingGames', [D_GameController::class, 'upcomingGames']);
    Route::get('/delegate/feedbackGames', [D_GameController::class, 'feedbackGames']);
    Route::get('/delegate/allPayments', [D_PaymentController::class, 'payments']);
    Route::get('/delegate/payments', [D_PaymentController::class, 'notApprovedPayments']);
    Route::get('/delegate/games/{id}', [D_GameController::class, 'show']);

    Route::get('/delegate/delegate_profile/{delegateId}', [D_ProfileController::class, 'getDelegate']);
    Route::get('/delegate/club_profile/{clubId}', [D_ProfileController::class, 'getClub']);
    Route::get('/delegate/referee_profile/{refereeId}', [D_ProfileController::class, 'getReferee']);

    Route::post('/delegate/controls', [D_ControlController::class, 'updateFeedback']);

    Route::post('/delegate/videos', [D_VideoController::class, 'addVideo']);

    Route::get('/delegate/payments/{paymentId}', [D_PaymentController::class, 'getPayment']);
    Route::post('/delegate/payments/{paymentId}/approve', [D_PaymentController::class, 'approvePayment']);
    Route::post('/delegate/payments/{paymentId}/decline', [D_PaymentController::class, 'declinePayment']);

    Route::post('/delegate/game/{id}/signUp', [D_GameController::class, 'signInToGame']);
    Route::post('/delegate/game/{id}/signOut', [D_GameController::class, 'signOutOfGame']);


    /** Referee routes (Matěj Lepeška) */
    //TODO


    /** Club routes (Matyáš Sapík) */
    //TODO
});

Route::post('/login', [AuthController::class, 'login']);
