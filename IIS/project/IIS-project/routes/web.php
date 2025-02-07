<?php

use Illuminate\Support\Facades\Route;
use Illuminate\Support\Facades\Auth;
use App\Http\Controllers\LoginController;
use App\Http\Controllers\RegisterController;
use App\Http\Controllers\PostController;
use App\Http\Controllers\WallController;
use App\Http\Controllers\ProfileController;
use App\Http\Controllers\SearchController;
use App\Http\Controllers\LikeController;
use App\Http\Controllers\CommentController;
use App\Http\Controllers\UserController;
use App\Http\Controllers\TagController;
use App\Http\Controllers\GroupController;
use App\Http\Middleware\GuestTimeout;
use App\Http\Middleware\CheckIfBanned;
use App\Http\Middleware\GuestAccess;

// Default route
Route::get('/', function () {
    if (Auth::check())
    {
        return redirect()->route('FYP');
    }
    return view('login');
});

// Authentication routes
Route::get('/login', [LoginController::class, 'showLoginForm'])->name('login');
Route::post('/login', [LoginController::class, 'login']);
Route::post('/logout', [LoginController::class, 'logout'])->name('logout');
Route::get('/guest-login', [LoginController::class, 'guestLogin'])->name('guest.login');
Route::middleware(['auth', 'guest.timeout'])->group(function () {
    // Guest-accessible routes
});
Route::get('/register', [RegisterController::class, 'showRegistrationForm'])->name('register');
Route::post('/register', [RegisterController::class, 'register']);

Route::middleware(['web', 'auth', GuestAccess::class, GuestTimeout::class, CheckIfBanned::class])->group(function () {
    // Main pages routes
    Route::get('/FYP', [WallController::class, 'showFYP'])->name('FYP');
    Route::get('/profile/{username}', [ProfileController::class, 'showUser'])->name('profile');
    Route::get('/search', [SearchController::class, 'showSearch'])->name('search');
    Route::get('/addPost', [PostController::class, 'showAddPost'])->name('addPost');
    Route::get('/groups', [GroupController::class, 'showMyGroups'])->name('myGroups');
    
    // FYP routes
    Route::post('/FYP/like', [LikeController::class, 'toggleLike'])->name('FYP.like');
    Route::post('/FYP/showComments', [CommentController::class, 'showComments'])->name('FYP.showComments'); //!
    Route::post('/FYP/addComment', [CommentController::class, 'addComment'])->name('FYP.addComment');
    Route::post('/FYP/removeComment', [CommentController::class, 'removeComment'])->name('FYP.removeComment');
    Route::post('/FYP/removeTag', [TagController::class, 'removeTag'])->name('FYP.removeTag'); //!
    Route::post('/FYP/removePost', [PostController::class, 'removePost'])->name('FYP.removePost');
    
    // Profile routes
    Route::get('/profile/post/{postId}', [WallController::class, 'showPost'])->name('profile.post.show');
    Route::post('/profile/banUser', [UserController::class, 'banUser'])->name('profile.banUser');
    Route::post('/profile/unbanUser', [UserController::class, 'unbanUser'])->name('profile.unbanUser'); //!
    Route::post('/profile/removeUser', [UserController::class, 'removeUser'])->name('profile.removeUser');
    Route::get('/profile/{username}/edit', [ProfileController::class, 'edit'])->name('profile.edit');
    Route::post('/profile/editData', [ProfileController::class, 'editData'])->name('profile.editData');
    Route::post('/profile/post/updateDescription', [PostController::class, 'updateDescription'])->name('profile.updateDescription');

    // Searching routes
    Route::post('/search/groups', [GroupController::class, 'showGroups'])->name('search.groups');
    Route::post('/search/users', [UserController::class, 'showUsers'])->name('search.users');
    Route::post('/search/posts', [TagController::class, 'showPosts'])->name('search.posts');
    Route::post('/search/groups/join', [GroupController::class, 'joinToGroup'])->name('search.groups.join');
    Route::post('/search/groups/remove', [GroupController::class, 'removeGroup'])->name('search.groups.remove');
    
    // Group list routes
    Route::post('/groups/addGroup', [GroupController::class, 'addGroup'])->name('groups.addGroup');
    Route::post('/groups/leave', [GroupController::class, 'leaveGroup'])->name('groups.leave');
    Route::get('/groups/{groupId}', [WallController::class, 'showGroupWall'])->name('groups.wall');
    Route::get('/groups/pendingRequests/{groupId}', [GroupController::class, 'showPendingRequests'])->name('groups.pendingRequests');
    Route::post('/groups/approve', [GroupController::class, 'approvePendingRequests'])->name('groups.approve');
    Route::post('/groups/refuse', [GroupController::class, 'refusePendingRequests'])->name('groups.refuse');

    // New post routes
    Route::post('/addNewPost', [PostController::class, 'addPost'])->name('addNewPost');
});
