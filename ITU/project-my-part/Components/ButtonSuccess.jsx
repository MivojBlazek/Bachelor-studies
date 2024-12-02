/**
 * ITU project
 * 
 * File: ButtonSuccess.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React, { useState } from 'react';
import color from '../../colors.jsx'

export default function ButtonSuccess({ label, onClick, style }) {
    const [isHovered, setIsHovered] = useState(false);

    return (
        <button
            onClick={onClick}
            onMouseEnter={() => setIsHovered(true)}
            onMouseLeave={() => setIsHovered(false)}
            style={{
                backgroundColor: isHovered ? color.success_hover : color.success,
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
