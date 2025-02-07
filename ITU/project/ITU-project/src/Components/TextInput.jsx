/**
 * ITU project
 * 
 * File: TextInput.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React from 'react';

export default function TextInput({ type = 'text', placeholder, reference }) {
    return (
        <input
            ref={reference}
            type={type}
            placeholder={placeholder}
            style={{
                padding: '10px',
                margin: '5px 0',
                width: '100%',
                boxSizing: 'border-box',
            }}
        />
    );
}
