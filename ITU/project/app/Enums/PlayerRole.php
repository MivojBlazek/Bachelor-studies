<?php

namespace App\Enums;

enum PlayerRole: string
{
    case Forward = 'forward';
    case Defend = 'defend';
    case Goalkeeper = 'goalkeeper';
}
