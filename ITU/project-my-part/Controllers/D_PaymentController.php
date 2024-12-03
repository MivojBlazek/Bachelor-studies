<?php
/**
 * ITU project
 * 
 * File: D_PaymentController.php
 * 
 * Author: Michal Blažek <xblaze38>
 */

namespace App\Http\Controllers\delegate;

use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use App\Models\Payment;
use Illuminate\Support\Facades\Auth;
use Carbon\Carbon;

class D_PaymentController extends Controller
{
    public function notApprovedPayments()
    {
        // Find payments that are not approved
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
            return response()->json(['error' => 'Payment not found.'], 404);
        }
        return response()->json($payment);
    }

    public function approvePayment($paymentId)
    {
        $delegate = Auth::user();

        $payment = Payment::find($paymentId);
        if (!$payment)
        {
            return response()->json(['error' => 'Payment not found.'], 404);
        }

        // Set approvalDate to current time and assign current user as approving delegate
        $payment->approvalDate = now();
        $payment->approved_by = $delegate->id;
        $payment->save();
        return response()->json(['success' => 'Payment approved successfully.'], 200);
    }

    public function declinePayment($paymentId)
    {
        $payment = Payment::find($paymentId);
        if (!$payment)
        {
            return response()->json(['error' => 'Payment not found.'], 404);
        }
        
        // Remove payment
        $payment->delete();
        return response()->json(['success' => 'Payment declined successfully.'], 200);
    }
}
