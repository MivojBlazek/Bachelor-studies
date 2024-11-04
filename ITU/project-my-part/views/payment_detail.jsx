import React, { useEffect, useState } from 'react';
import { useParams, useNavigate } from 'react-router-dom';
import axiosClient from '../../axiosClient';
import GameCard from '../../Components/delegate/GameCard';
import ButtonSuccess from '../../Components/delegate/ButtonSuccess';
import ButtonFailure from '../../Components/delegate/ButtonFailure';
import P from '../../Components/delegate/P';

export default function PaymentDetail() {
    const { paymentId } = useParams();
    const [payment, setPayment] = useState(null);
    const [error, setError] = useState(null);
    const navigate = useNavigate();

    useEffect(() => {
        const fetchPayment = async () => {
            try
            {
                const response = await axiosClient.get(`/delegate/payments/${paymentId}`);
                setPayment(response.data);
            }
            catch (error)
            {
                setError(error.message);
            }
        };

        fetchPayment();
    }, [paymentId]);

    const approve = async () => {
        try
        {
            await axiosClient.post(`/delegate/payments/${paymentId}/approve`);
            navigate(`/delegate/dashboard`);
        }
        catch (error)
        {
            setError(error.response.data.error || 'An error occurred while approving the payment');
        }
    }

    const decline = async () => {
        try
        {
            await axiosClient.post(`/delegate/payments/${paymentId}/decline`);
            navigate(`/delegate/dashboard`);
        }
        catch (error)
        {
            setError(error.response.data.error || 'An error occurred while declining the payment');
        }
    }

    if (!payment)
    {
        return null;
    }

    return (
        <div style={{ padding: '20px', textAlign: 'center' }}>
            <h1>
                <P label={`Referee: ${payment.control.referee.name}`} href={`/delegate/referee_profile/${payment.control.referee.id}`} />
            </h1>
            <p style={{ fontWeight: 'bold' }}>Date created: {payment.createdAt}</p>
            <p style={{ fontWeight: 'bold' }}>Amount: {payment.amount}czk</p>
            <GameCard game={payment.control.game} />
            {payment.approved_by === null && (
                <>
                    <ButtonSuccess
                        label='Approve'
                        onClick={approve}
                    />
                    <ButtonFailure
                        label='Decline'
                        onClick={decline}
                    />
                </>
            )}
        </div>
    );
}