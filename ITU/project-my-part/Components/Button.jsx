import React from 'react';
import color from '../../colors.jsx'

export default function Button({ label, onClick}) {
    return (
        <button
            onClick={onClick}
            style={{
                padding: '10px',
                backgroundColor: color.primary,
                color: 'white',
                border: 'none',
                cursor: 'pointer',
                width: '100%',
                marginTop: '10px',
            }}
        >
            {label}
        </button>
    );
}
