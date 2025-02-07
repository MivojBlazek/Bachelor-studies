/**
 * ITU project
 * 
 * File: ErrorMessage.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React from 'react';

export default function ErrorMessage({ message }) {
    return (
        message && (
            <div
                style={{
                    color: 'red',
                    backgroundColor: '#ffe6e6',
                    padding: '10px',
                    marginTop: '10px',
                    borderRadius: '5px',
                }}
            >
                {message}
            </div>
        )
    );
}
