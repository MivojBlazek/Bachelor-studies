import React from 'react';

export default function SuccessMessage({ message }) {
    return (
        message && (
            <div
                style={{
                    color: 'green',
                    backgroundColor: '#C5FCC3',
                    padding: '10px',
                    marginTop: '10px',
                    marginBottom: '10px',
                    borderRadius: '5px',
                }}
            >
                {message}
            </div>
        )
    );
}
