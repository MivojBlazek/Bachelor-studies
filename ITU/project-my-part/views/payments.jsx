/**
 * ITU project
 * 
 * File: payments.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React, { useEffect, useState } from 'react';
import PaymentCard from '../../Components/delegate/PaymentCard.jsx';
import axiosClient from '../../axiosClient.js';
import FilterPayments from '../../Components/delegate/FilterPayments.jsx';
import ErrorMessage from '../../Components/delegate/ErrorMessage.jsx';

export default function Payments() {
    const [payments, setPayments] = useState([]);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchPayments = async () => {
            try
            {
                const response = await axiosClient.get('/delegate/allPayments');
                if (Array.isArray(response.data))
                {
                    setPayments(response.data);
                    setError(null);
                }
                else
                {
                    setPayments([]);
                    setError('No payments found.');
                }
            }
            catch (error)
            {
                setPayments([]);
                setError('Failed to fetch payments.');
            }
        };
    
        fetchPayments();
    }, []);

    const onFilter = async (params) => {
        try
        {
            const response = await axiosClient.get('/delegate/allPayments', { params: params });
            if (response.data.length > 0)
            {
                setPayments(response.data);
                setError(null);
            }
            else
            {
                setPayments([]);
                setError('No payments found with current filters.');
            }
        }
        catch (error)
        {
            setPayments([]);
            setError('Failed to fetch payments.');
        }
    }

    return (
        <div style={{margin: '0px 20px', textAlign: 'center'}}>
            <FilterPayments onFilter={onFilter} />
            <div>
                <ErrorMessage message={error} />
                {payments.map(payment => (
                    <PaymentCard 
                        key={payment.id}
                        payment={payment}
                    />
                ))}
            </div>
        </div>
    );
}
