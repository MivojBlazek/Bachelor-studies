<?php
/**
 * ITU project
 * 
 * File: FeedbackRequest.php
 * 
 * Author: Michal Blažek <xblaze38>
 */

namespace App\Http\Requests\delegate;

use Illuminate\Foundation\Http\FormRequest;
use Illuminate\Validation\Rules\Password;

class FeedbackRequest extends FormRequest
{
    /**
     * Determine if the user is authorized to make this request.
     */
    public function authorize(): bool
    {
        return true;
    }

    /**
     * Get the validation rules that apply to the request.
     *
     * @return array<string, \Illuminate\Contracts\Validation\ValidationRule|array<mixed>|string>
     */
    public function rules(): array
    {
        return [
            'controlId' => 'required|exists:controls,id',
            'feedback' => 'string|nullable',
        ];
    }
}
