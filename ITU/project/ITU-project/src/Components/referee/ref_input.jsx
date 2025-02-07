/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React, { useState } from 'react';

export default function RefInput({ type = 'text', placeholder, value, onChange, onBlur, reference }) {
    const [isFocused, setIsFocused] = useState(false);

    return (
        <input
            ref={reference}
            type={type}
            placeholder={placeholder}
            value={value}
            onChange={onChange}
            onBlur={(e) => {
                setIsFocused(false);
                if (onBlur) onBlur(e);
            }}
            onFocus={() => setIsFocused(true)}
            style={{
                padding: isFocused ? '15px' : '10px',
                margin: '5px 0',
                width: '50vw',
                boxSizing: 'border-box',
                border: isFocused ? '2px solid #007BFF' : '1px solid #ccc',
                borderRadius: '5px',
                outline: 'none',
                transition: 'all 0.1s ease-in-out',
                backgroundColor: isFocused ? '#f2efdc' : '#f7f5e9',
                fontSize: 'clamp(10px, 2.2vh, 30px)',
            }}
        />
    );
}
