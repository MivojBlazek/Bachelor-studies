<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Models\Payment;

class PaymentController extends Controller
{
    public function payments()
    {
        $unapprovedPayments = Payment::with(['control.referee', 'delegate'])->where('approvalDate', null)->get();
        return response()->json($unapprovedPayments);
    }
}
