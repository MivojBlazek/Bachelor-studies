<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;
use App\Enums\RefereeRole;
use App\Models\Payment;
use App\Models\Control;
use App\Models\Delegate;

class PaymentSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $control = Control::first();
        $control2 = Control::where('feedback', '')->first();
        $control3 = Control::where('feedback', '')->where('refereeRole', RefereeRole::Line)->first();
        $delegate = Delegate::first();

        Payment::factory()->create([
            'amount' => 1000,
            'createdAt' => now(),
            'approvalDate' => today(),
            'control_id' => $control->id,
            'approved_by' => $delegate->id,
        ]);

        Payment::factory()->create([
            'amount' => 500,
            'createdAt' => now(),
            'approvalDate' => null,
            'control_id' => $control2->id,
            'approved_by' => null,
        ]);

        Payment::factory()->create([
            'amount' => 2500,
            'createdAt' => now(),
            'approvalDate' => null,
            'control_id' => $control3->id,
            'approved_by' => null,
        ]);
    }
}
