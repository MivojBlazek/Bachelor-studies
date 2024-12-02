/**
 * ITU project
 * 
 * File: PaymentCard.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React from 'react';
import { useNavigate } from 'react-router-dom';
import Card from './Card.jsx';

export default function PaymentCard({ payment }) {
    const navigate = useNavigate();

    const goToDetail = (ev) => {
        ev.preventDefault();
        navigate(`/delegate/payment/${payment.id}`);
    };

    const cardContent = (
        <div
            style={{
                margin: '0 auto',
                width: 'fit-content',
                display: 'grid',
                gridTemplateColumns: 'auto auto',
                gap: '13px',
                textAlign: 'left',
                padding: '15px',
            }}
        >
            <h2 style={{ textAlign: 'right', margin: '0px' }}>Referee:</h2><h2 style={{ margin: '0px' }}>{payment.control.referee.name}</h2>
            <p style={{ textAlign: 'right', margin: '0px' }}>Date created:</p><p style={{ margin: '0px' }}>{(new Date(payment.created_at)).toLocaleDateString()} {(new Date(payment.created_at)).toLocaleTimeString()}</p>
            <p style={{ textAlign: 'right', margin: '0px' }}>Amount:</p><p style={{ margin: '0px' }}>{payment.amount}czk</p>
        </div>
    );

    return (
        <div>
            <Card onClick={goToDetail} content={cardContent} />
        </div>
    );
}
