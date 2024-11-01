import React from 'react';

export default function Button({ label, onClick, color = '#2244BB'}) {
    return (
        <button
            onClick={onClick}
            style={{
                padding: '10px',
                backgroundColor: color,
                color: 'white',
                border: 'none',
                borderRadius: '20px',
                cursor: 'pointer',
                width: '100%',
                marginTop: '10px',
            }}
        >
            {label}
        </button>
    );
}
