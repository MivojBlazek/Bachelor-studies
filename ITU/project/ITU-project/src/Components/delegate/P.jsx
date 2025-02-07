/**
 * ITU project
 * 
 * File: P.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React from 'react';

export default function P({ label, href, style }) {
    return (
        <a
            href={href}
            style={{
                color: 'black',
                textDecoration: 'none',
                ...style,
            }}
        >
            {label}
        </a>
    );
}
