/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';

export default function Filters({ filters, onFilterChange }) {
    const styles = {
        container: {
            marginBottom: '20px',
            display: 'flex',
            gap: '10px',
            flexWrap: 'wrap',
        },
        input: {
            width: '11vw', 
            padding: '8px',
            borderRadius: '4px',
            border: '1px solid #ccc',
            fontSize: 'clamp(10px, 2vh, 20px)',
            flex: '1 1 150px',
        },
    };

    return (
        <div>
            <div style={styles.container}>
                <input
                    type="date"
                    name="date"
                    value={filters.date}
                    onChange={onFilterChange}
                    placeholder="Date"
                    style={styles.input}
                />
                <input
                    type="text"
                    name="location"
                    value={filters.location}
                    onChange={onFilterChange}
                    placeholder="Location"
                    style={styles.input}
                />
                <input
                    type="text"
                    name="club"
                    value={filters.club}
                    onChange={onFilterChange}
                    placeholder="Club"
                    style={styles.input}
                />
                <input
                    type="text"
                    name="referee"
                    value={filters.referee}
                    onChange={onFilterChange}
                    placeholder="Referee name"
                    style={styles.input}
                />
                <input
                    type="text"
                    name="league"
                    value={filters.league}
                    onChange={onFilterChange}
                    placeholder="League"
                    style={styles.input}
                />
            </div>
        </div>
    );
}
