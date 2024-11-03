import React from 'react';

export default function InputWithLabel({ label, type='text', value, onChange }) {
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
                type={type}
                value={value}
                onChange={onChange}
                style={{
                    padding: '5px',
                    maxWidth: '150px',
                }}
            />
        </div>
    );
}
