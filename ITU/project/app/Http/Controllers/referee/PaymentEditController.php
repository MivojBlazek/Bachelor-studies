<?php
/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

namespace App\Http\Controllers\referee;

use App\Models\Payment;
use Illuminate\Http\Request;
use Carbon\Carbon;
use App\Http\Controllers\Controller;

class PaymentEditController extends Controller
{
    public function paymentStore(Request $request)
    {
        
        $validated = $request->validate([
            'amount' => 'required|numeric',
            'control_id' => 'required|integer',
        ]);

        $payment = Payment::create([
            'amount' => $validated['amount'],
            'createdAt' => Carbon::now(),
            'approvalDate' => null,
            'approved_by' => null,
            'control_id' => $validated['control_id'],
        ]);

        return response()->json([
            'message' => 'Payment created successfully',
            'payment' => $payment,
        ], 201);
    }
}
