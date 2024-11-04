<?php

namespace App\Http\Controllers\delegate;

use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use App\Models\Payment;
use Illuminate\Support\Facades\Auth;
use Carbon\Carbon;

class PaymentController extends Controller
{
    public function notApprovedPayments()
    {
        $unapprovedPayments = Payment::with(['control.referee', 'delegate'])->where('approvalDate', null)->get();
        return response()->json($unapprovedPayments);
    }

    public function payments(Request $request)
    {
        $query = Payment::with(['control.referee', 'delegate']);

        // Filters
        if ($request->has('dateFrom') && !empty($request->input('dateFrom')))
        {
            $query->where('createdAt', '>=', Carbon::parse($request->input('dateFrom')));
        }
        if ($request->has('dateTo') && !empty($request->input('dateTo')))
        {   
            $query->where('createdAt', '<=', Carbon::parse($request->input('dateTo')));
        }
        if ($request->has('referee') && !empty($request->input('referee')))
        {
            $query->whereHas('control.referee', function ($q) use ($request) {
                $q->where('name', 'like', '%' . $request->input('referee') . '%');
            });
        }
        if ($request->has('notApproved'))
        {
            if ($request->input('notApproved') === 'true')
            {
                $query->where('approvalDate', null);
            }
        }

        $payments = $query->get();
        return response()->json($payments);
    }

    public function getPayment($paymentId)
    {
        $payment = Payment::with(['control.referee', 'control.game', 'control.game.club1', 'control.game.club2', 'delegate'])->find($paymentId);
        if (!$payment)
        {
            return response()->json(['error' => 'Payment not found'], 404);
        }
        return response()->json($payment);
    }

    public function approvePayment($paymentId)
    {
        $delegate = Auth::user();

        $payment = Payment::find($paymentId);
        if (!$payment)
        {
            return response()->json(['error' => 'Payment not found'], 404);
        }

        $payment->approvalDate = now();
        $payment->approved_by = $delegate->id;
        $payment->save();
    }

    public function declinePayment($paymentId)
    {
        $payment = Payment::find($paymentId);
        if (!$payment)
        {
            return response()->json(['error' => 'Payment not found'], 404);
        }
        
        $payment->delete();
    }
}
