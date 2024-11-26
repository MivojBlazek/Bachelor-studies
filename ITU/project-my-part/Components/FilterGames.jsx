import React, { useState } from 'react';
import color from '../../colors.jsx'
import InputWithLabel from './InputWithLabel.jsx';
import Button from './Button.jsx';

export default function FilterGames({ onFilter }) {
    const [dateFrom, setDateFrom] = useState('');
    const [dateTo, setDateTo] = useState('');
    const [location, setLocation] = useState('');
    const [league, setLeague] = useState('');

    const filter = () => {
        onFilter({ dateFrom, dateTo, location, league });
    };

    return (
        <div
            style={{
                display: 'flex',
                flexDirection: 'row',
                flexWrap: 'wrap',
                gap: '5px',
                justifyContent: 'center',
                alignItems: 'center',
                padding: '10px',
                backgroundColor: color.background,
            }}
        >
            <InputWithLabel
                label='Date from:'
                type='date'
                value={dateFrom}
                onChange={(e) => setDateFrom(e.target.value)}
            />
            <InputWithLabel
                label='Date to:'
                type='date'
                value={dateTo}
                onChange={(e) => setDateTo(e.target.value)}
            />
            <InputWithLabel
                label='Location:'
                type='text'
                value={location}
                onChange={(e) => setLocation(e.target.value)}
            />
            <InputWithLabel
                label='League:'
                type='text'
                value={league}
                onChange={(e) => setLeague(e.target.value)}
            />
            <Button
                label='Filter'
                onClick={filter}
            />
        </div>
    );
}