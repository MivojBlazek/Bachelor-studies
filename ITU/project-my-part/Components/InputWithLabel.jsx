import React from 'react';

export default function InputWithLabel({ label, type='text', value, onChange, inputStyle}) {
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
                placeholder={label}
                type={type}
                value={value}
                onChange={onChange}
                style={{
                    padding: '5px',
                    maxWidth: '150px',  
                    ...inputStyle
                }}
            />
        </div>
    );
}
