<?php

namespace App\Listeners;

use Illuminate\Contracts\Queue\ShouldQueue;
use Illuminate\Queue\InteractsWithQueue;
use Illuminate\Auth\Events\Logout;

class DeleteGuestOnLogout
{
    /**
     * Handle the event.
     */
    public function handle(Logout $event)
    {
        $user = $event->user;
        if ($user && $user->rights === 'guest') {
            $user->delete();
        }
    }
}
