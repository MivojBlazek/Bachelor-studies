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
use App\Http\Controllers\club\ClubController;
use App\Http\Controllers\club\ClubMatchController;
use App\Http\Controllers\club\ClubPlayerController;
use App\Http\Controllers\referee\ProfileController;
use App\Http\Controllers\referee\DashboardController;
use App\Http\Controllers\referee\PlayedMatchesController;
use App\Http\Controllers\referee\RegisteredMatchesController;
use App\Http\Controllers\referee\GameControlController;
use App\Http\Controllers\referee\AllMatchesController;
use App\Http\Controllers\referee\AddControlController;
use App\Http\Controllers\referee\PaymentsController;
use App\Http\Controllers\referee\GameDetailController;
use App\Http\Controllers\referee\PaymentEditController;
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
    Route::get('/referee/profile', [ProfileController::class, 'show']);

    Route::put('/referee/profile', [ProfileController::class, 'update']);

    Route::get('/referee/dashboard', [DashboardController::class, 'getAssignedGames']);

    Route::get('/referee/played_matches', [PlayedMatchesController::class, 'getPlayedGames']);

    Route::get('/referee/registered_matches', [RegisteredMatchesController::class, 'getRegisteredGames']);

    Route::delete('/referee/controls', [GameControlController::class, 'deleteControl']);

    Route::get('/referee/all_matches', [AllMatchesController::class, 'getAllGames']);

    Route::post('referee/controls/add', [AddControlController::class, 'addControl']);

    Route::get('/referee/payments', [PaymentsController::class, 'getPayment']);

    Route::get('/referee/game_detail/{id}', [GameDetailController::class, 'GetGameDetail']);

    Route::post('referee/payments/edit', [PaymentEditController::class, 'paymentStore']);


    /** Club routes (Matyáš Sapík) */
    // Club Profile
    Route::get('/club/profileshow', [ClubController::class, 'show']);
    Route::post('/club/profileupdate', [ClubController::class, 'update']);

    // Club Players
    Route::get('/club/player/playershow', [ClubPlayerController::class, 'show']);
    Route::put('/club/player/{id}', [ClubPlayerController::class, 'update']);
    Route::post('/club/player/playercreate', [ClubPlayerController::class, 'create']);
    Route::delete('/club/player/{id}', [ClubPlayerController::class, 'delete']);

    // Club Matches
    Route::get('/club/match/{id}', [ClubMatchController::class, 'show']);
    Route::put('/club/match/{id}', [ClubMatchController::class, 'update']);
    Route::get('/club/clubs', [ClubController::class, 'index']); // Route pro seznam klubů
    Route::post('/club/matchescreate', [ClubMatchController::class, 'create']);
    Route::delete('/club/match/{id}', [ClubMatchController::class, 'destroy']);

    Route::get('/club/board/upcoming', [ClubMatchController::class, 'show']);
});

Route::post('/login', [AuthController::class, 'login']);
