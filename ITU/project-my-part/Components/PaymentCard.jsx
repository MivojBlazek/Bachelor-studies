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
        <>
            <h2>Referee: {payment.control.referee.name}</h2>
            <p>Date created: {payment.createdAt}</p>
            <p>Amount: {payment.amount}czk</p>
        </>
    );

    return (
        <div>
            <Card onClick={goToDetail} content={cardContent} />
        </div>
    );
}
