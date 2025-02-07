<?php

namespace Database\Factories;

use Illuminate\Database\Eloquent\Factories\Factory;
use App\Models\Control;
use App\Models\Delegate;

/**
 * @extends \Illuminate\Database\Eloquent\Factories\Factory<\App\Models\Payment>
 */
class PaymentFactory extends Factory
{
    /**
     * Define the model's default state.
     *
     * @return array<string, mixed>
     */
    public function definition(): array
    {
        return [
            'amount' => 0,
            'createdAt' => now(),
            'approvalDate' => today(),
            'control_id' => Control::factory(),
            'approved_by' => Delegate::factory(),
        ];
    }
}
