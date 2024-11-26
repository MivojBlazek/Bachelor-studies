import React, { useState } from 'react';
import color from '../../colors.jsx'
import InputWithLabel from './InputWithLabel.jsx';
import Button from './Button.jsx';

export default function FilterPayments({ onFilter }) {
    const [dateFrom, setDateFrom] = useState('');
    const [dateTo, setDateTo] = useState('');
    const [referee, setReferee] = useState('');
    const [notApproved, setNotApproved] = useState(false);

    const filter = () => {
        onFilter({ dateFrom, dateTo, referee, notApproved });
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
                label='Referee:'
                type='text'
                value={referee}
                onChange={(e) => setReferee(e.target.value)}
            />
            <InputWithLabel
                label='Not approved:'
                type='checkbox'
                checked={notApproved}
                onChange={(e) => setNotApproved(e.target.checked)}
                inputStyle={{
                    width: '25px',
                    height: '25px',
                }}
            />
            <Button
                label='Filter'
                onClick={filter}
            />
        </div>
    );
}