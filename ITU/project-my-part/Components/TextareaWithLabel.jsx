/**
 * ITU project
 * 
 * File: TextareaWithLabel.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React from 'react';

export default function TextareaWithLabel({ label, value, onChange, placeholder = null}) {
    const resizeTextarea = (textarea) => {
        if (textarea)
        {
            textarea.style.height = 'auto';
            textarea.style.height = `${textarea.scrollHeight}px`;
        }
    };

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
            <textarea
                placeholder={placeholder ? placeholder : label}
                value={value}
                onChange={onChange}
                onInput={(e) => resizeTextarea(e.target)}
                style={{
                    padding: '5px',
                    width: '100%',
                    minHeight: '15px',
                    fontSize: '14px',
                    resize: 'none',
                    overflow: 'hidden',
                }}
            />
        </div>
    );
}
