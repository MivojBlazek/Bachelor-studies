/**
 * ITU project
 * 
 * File: Card.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React, { useState } from 'react';
import color from '../../colors.jsx'

export default function Card({ onClick, content }) {
    const [isHovered, setIsHovered] = useState(false);

    return (
        <div
            style={{
                maxWidth: '800px',
                margin: '20px auto',
                border: 'none',
                borderRadius: '10px',
                boxShadow: '2px 4px 5px rgba(0, 0, 0, 0.3)',
                backgroundColor: isHovered ? color.hover : color.primary,
                transition: 'background-color 0.3s ease',
            }}
        >
            <a
                href='#'
                onClick={onClick}
                onMouseEnter={() => setIsHovered(true)}
                onMouseLeave={() => setIsHovered(false)}
                style={{
                    textDecoration: 'none',
                    color: 'black',
                    padding: '10px 20px',
                    margin: '20px',
                }}
            >
                {content}
            </a>
        </div>
    );
}
