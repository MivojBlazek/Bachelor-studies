/**
 * ITU project
 * 
 * File: payment_detail.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React, { useEffect, useState } from 'react';
import { useParams, useNavigate } from 'react-router-dom';
import axiosClient from '../../axiosClient';
import GameCard from '../../Components/delegate/GameCard';
import ButtonSuccess from '../../Components/delegate/ButtonSuccess';
import ButtonFailure from '../../Components/delegate/ButtonFailure';
import P from '../../Components/delegate/P';
import ErrorMessage from '../../Components/delegate/ErrorMessage';

export default function PaymentDetail() {
    const { paymentId } = useParams();
    const [payment, setPayment] = useState(null);
    const [error, setError] = useState(null);
    const [success, setSuccess] = useState(null);
    const navigate = useNavigate();

    // Fetch payment detail from url
    useEffect(() => {
        const fetchPayment = async () => {
            try
            {
                const response = await axiosClient.get(`/delegate/payments/${paymentId}`);
                setPayment(response.data);
                setError(null);
                setSuccess(null);
            }
            catch (error)
            {
                if (error.response)
                {
                    setError(error.response.data.error);
                }
                else
                {
                    setError(error.message);
                }
                setSuccess(null);
            }
        };

        fetchPayment();
    }, [paymentId]);

    // If approve button was clicked, show confirming alert window, approve and redirect to dashboard
    const approve = async () => {
        const confirmed = window.confirm('Are you sure you want to approve this payment?');
        if (!confirmed)
        {
            return;
        }

        try
        {
            const response = await axiosClient.post(`/delegate/payments/${paymentId}/approve`);
            navigate(`/delegate/dashboard`, { state: { success: response.data.success } });
        }
        catch (error)
        {
            setError(error.response?.data?.error || 'An error occurred while approving the payment');
            setSuccess(null);
        }
    }

    // If decline button was clicked, show confirming alert window, decline and redirect to dashboard
    const decline = async () => {
        const confirmed = window.confirm('Are you sure you want to decline this payment?');
        if (!confirmed)
        {
            return;
        }

        try
        {
            const response = await axiosClient.post(`/delegate/payments/${paymentId}/decline`);
            navigate(`/delegate/dashboard`, { state: { success: response.data.success } });
        }
        catch (error)
        {
            setError(error.response?.data?.error || 'An error occurred while declining the payment');
            setSuccess(null);
        }
    }

    if (!payment && !error)
    {
        return null;
    }

    return (
        <>
            {!payment && <ErrorMessage message={error} />}
            {payment && (
                <div style={{ padding: '20px', textAlign: 'center' }}>
                    <div
                        style={{
                            margin: '0 auto',
                            width: 'fit-content',
                            fontWeight: 'bold',
                            display: 'grid',
                            gridTemplateColumns: 'auto auto',
                            gap: '13px',
                            textAlign: 'left',
                        }}
                    >
                        {/* Payment information */}
                        <h1 style={{ textAlign: 'right', margin: '0px 0px 15px 0px' }}>Referee:</h1><h1 style={{ margin: '0px 0px 15px 0px' }}><P label={`${payment.control.referee.name}`} href={`/delegate/referee_profile/${payment.control.referee.id}`}/></h1>
                        <p style={{ textAlign: 'right', margin: '0px' }}>Date created:</p><p style={{ margin: '0px' }}>{(new Date(payment.created_at)).toLocaleDateString()} {(new Date(payment.created_at)).toLocaleTimeString()}</p>
                        <p style={{ textAlign: 'right', margin: '0px' }}>Amount:</p><p style={{ margin: '0px' }}>{payment.amount}czk</p>
                        {/* If payment is already approved, show delegate, who approved it, and date */}
                        {payment.approved_by !== null &&
                            <>
                                <p style={{ textAlign: 'right', margin: '0px' }}>Approved by:</p><P label={`${payment.delegate.name}`} href={`/delegate/delegate_profile/${payment.delegate.id}`} />
                                <p style={{ textAlign: 'right', margin: '0px' }}>Date of approvement:</p><p style={{ margin: '0px' }}>{(new Date(payment.approvalDate)).toLocaleDateString()}</p>
                            </>
                        }
                    </div>
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
            )}
        </>
    );
}