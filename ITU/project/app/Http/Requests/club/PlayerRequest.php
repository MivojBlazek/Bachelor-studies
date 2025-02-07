<?php
/**
 * ITU project
 * 
 * Author: Matyáš Sapík <xsapik02>
 */

namespace App\Http\Requests\club;

use Illuminate\Foundation\Http\FormRequest;

class PlayerRequest extends FormRequest
{
    /**
     * Determine if the user is authorized to make this request.
     */
    public function authorize(): bool
    {
        return true; // Ensure proper authorization logic is handled elsewhere.
    }

    /**
     * Get the validation rules that apply to the request.
     *
     * @return array<string, \Illuminate\Contracts\Validation\ValidationRule|array<mixed>|string>
     */
    public function rules(): array
    {
        $playerId = $this->route('id'); // Retrieve the player ID from the route

        return [
            'name' => ['nullable', 'string', 'max:255'], // Name can be null or a string
            'dress_number' => ['nullable', 'integer', 'min:1', 'max:255'], // Dress number can be null
            'role' => ['nullable', 'string', 'in:forward,defend,goalkeeper,new'], // Role can be null or one of the valid roles
            'address' => ['nullable', 'string', 'max:255'], // Address can be null or a string
            'phone' => ['nullable', 'string', 'max:20'], // Phone can be null or a string
            'email' => ['nullable', 'email', 'max:255', "unique:players,email,{$playerId}"], // Email can be null or unique for other players
        ];
    }
}
