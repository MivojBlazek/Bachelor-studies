<?php

namespace App\Providers;

use Illuminate\Support\ServiceProvider;
use Illuminate\Support\Facades\Event;
use Illuminate\Auth\Events\Logout;
use App\Listeners\DeleteGuestOnLogout;

class AppServiceProvider extends ServiceProvider
{
    /**
     * Register any application services.
     */
    public function register(): void
    {
        //
    }

    /**
     * Bootstrap any application services.
     */
    public function boot(): void
    {
        // Register the logout event listener for guests
        Event::listen(
            Logout::class,
            [DeleteGuestOnLogout::class, 'handle']
        );
    }
}
