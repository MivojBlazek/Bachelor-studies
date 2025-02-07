/**
 * ITU project
 * 
 * File: InputWithLabel.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React from 'react';

export default function InputWithLabel({ label, type='text', value, onChange, inputStyle, placeholder = null}) {
    return (
        <div>
            <label
                style={{
                    display: 'block',
                    fontWeight: 'bold',
                    marginBottom: '5px',
                }}
            >
                {label}
            </label>
            <input
                placeholder={placeholder ? placeholder : label}
                type={type}
                value={value}
                onChange={onChange}
                style={{
                    padding: '5px',
                    fontSize: '14px',
                    maxWidth: '150px',  
                    ...inputStyle
                }}
            />
        </div>
    );
}
