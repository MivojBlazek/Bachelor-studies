<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Auth;

class SearchController extends Controller
{
    public function showSearch()
    {
        $user = Auth::user();
        return view('search', compact('user'));
    }
}
