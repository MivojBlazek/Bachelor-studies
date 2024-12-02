/**
 * ITU project
 * 
 * File: ButtonFailure.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React, { useState } from 'react';
import color from '../../colors.jsx'

export default function ButtonFailure({ label, onClick, style }) {
    const [isHovered, setIsHovered] = useState(false);

    return (
        <button
            onClick={onClick}
            onMouseEnter={() => setIsHovered(true)}
            onMouseLeave={() => setIsHovered(false)}
            style={{
                backgroundColor: isHovered ? color.failure_hover : color.failure,
                padding: '10px 20px',
                border: 'none',
                borderRadius: '20px',
                margin: '10px',
                boxShadow: '2px 4px 5px rgba(0, 0, 0, 0.3)',
                transition: 'background-color 0.3s ease',
                ...style
            }}
        >
            {label}
        </button>
    );
}
