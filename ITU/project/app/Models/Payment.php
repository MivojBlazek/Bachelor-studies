<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Notifications\Notifiable;
use Laravel\Sanctum\HasApiTokens;

class Payment extends Model
{
    /** @use HasFactory<\Database\Factories\PaymentFactory> */
    use HasApiTokens, HasFactory, Notifiable;
    
    protected $table = 'payments';

    protected $fillable = [
        'amount',
        'createdAt',
        'approvalDate',
        'control_id',
        'approved_by',
    ];

    public function control()
    {
        return $this->belongsTo(Control::class);
    }

    public function delegate()
    {
        return $this->belongsTo(Delegate::class, 'approved_by');
    }
}
