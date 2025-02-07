<?php
/**
 * ITU project
 * 
 * Author: Matyáš Sapík <xsapik02>
 */

namespace App\Http\Requests\club;

use Illuminate\Foundation\Http\FormRequest;

class MatchRequest extends FormRequest
{
    /**
     * Determine if the user is authorized to make this request.
     */
    public function authorize(): bool
    {
        // Toto může být aktualizováno pro lepší kontrolu oprávnění.
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
            'date' => 'required|date|after_or_equal:today', // Datum musí být dnešní nebo budoucí
            'time' => 'required|date_format:H:i:s',         // Čas ve formátu H:i:s
            'location' => 'required|string|max:255',       // Maximální délka pro lokaci je 255 znaků
            'league' => 'required|string|max:255',         // Maximální délka ligy je 255 znaků
            'clubHome_id' => 'required|exists:clubs,id',    // ID domácího klubu musí existovat v tabulce clubs
            'clubVisitor_id' => 'required|exists:clubs,id|different:clubHome_id', // ID hostujícího klubu musí být jiné než domácí
        ];
    }

    /**
     * Custom error messages for validation rules.
     *
     * @return array<string, string>
     */
    public function messages(): array
    {
        return [
            'date.required' => 'The date of the match is required.',
            'date.after_or_equal' => 'The match date must be today or a future date.',
            'time.required' => 'The time of the match is required.',
            'time.date_format' => 'The time format must be H:i:s.',
            'location.required' => 'The match location is required.',
            'league.required' => 'The league is required.',
            'clubHome_id.required' => 'The home team is required.',
            'clubHome_id.exists' => 'The selected home team does not exist.',
            'clubVisitor_id.required' => 'The visitor team is required.',
            'clubVisitor_id.exists' => 'The selected visitor team does not exist.',
            'clubVisitor_id.different' => 'The visitor team must be different from the home team.',
        ];
    }
}
