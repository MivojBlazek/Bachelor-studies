import React, { useEffect, useState } from 'react';
import PaymentCard from '../../Components/delegate/PaymentCard.jsx';
import axiosClient from '../../axiosClient.js';
import FilterPayments from '../../Components/delegate/FilterPayments.jsx';

export default function Payments() {
    const [payments, setPayments] = useState([]);

    useEffect(() => {
        const fetchPayments = async () => {
            try
            {
                const response = await axiosClient.get('/delegate/allPayments');
                if (Array.isArray(response.data))
                {
                    setPayments(response.data);
                }
                else
                {
                    setPayments([]);
                }
            }
            catch (error)
            {
                setPayments([]);
            }
        };
    
        fetchPayments();
    }, []);

    const onFilter = async (params) => {
        try
        {
            const response = await axiosClient.get('/delegate/allPayments', { params: params });
            setPayments(response.data);
        }
        catch (error)
        {
            setPayments([]);
        }
    }

    return (
        <div style={{margin: '0px 20px', textAlign: 'center'}}>
            <FilterPayments onFilter={onFilter} />
            <div>
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
