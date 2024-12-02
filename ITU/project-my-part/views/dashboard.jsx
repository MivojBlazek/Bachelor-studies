/**
 * ITU project
 * 
 * File: dashboard.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React, { useEffect, useState } from 'react';
import { useLocation } from 'react-router-dom';
import GameCard from '../../Components/delegate/GameCard.jsx';
import FeedbackCard from '../../Components/delegate/FeedbackCard.jsx';
import PaymentCard from '../../Components/delegate/PaymentCard.jsx';
import axiosClient from '../../axiosClient.js';
import SuccessMessage from '../../Components/delegate/SuccessMessage.jsx'

export default function Dashboard() {
    const [upcomingGames, setGames] = useState([]);
    const [feedbackGames, setFeedbackGames] = useState([]);
    const [payments, setPayments] = useState([]);
    const [success, setSuccess] = useState(null);
    const location = useLocation();

    useEffect(() => {
        if (location.state?.success)
        {
            setSuccess(location.state.success);
        }
    }, [location.state]);

    useEffect(() => {
        const fetchGames = async () => {
            try
            {
                const response = await axiosClient.get('/delegate/upcomingGames');
                if (Array.isArray(response.data))
                {
                    setGames(response.data);
                }
                else
                {
                    setGames([]);
                }
            }
            catch (error)
            {
                setGames([]);
            }
        };
    
        fetchGames();
    }, []);
    
    useEffect(() => {
        const fetchFeedbackGames = async () => {
            try
            {
                const response = await axiosClient.get('/delegate/feedbackGames');
                if (Array.isArray(response.data))
                {
                    setFeedbackGames(response.data);
                }
                else
                {
                    setFeedbackGames([]);
                }
            }
            catch (error)
            {
                setFeedbackGames([]);
            }
        };
    
        fetchFeedbackGames();
    }, []);

    useEffect(() => {
        const fetchPayments = async () => {
            try
            {
                const response = await axiosClient.get('/delegate/payments');
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

    return (
        <div style={{margin: '0px 20px', textAlign: 'center'}}>
            {upcomingGames.length === 0 && feedbackGames.length === 0 && payments.length === 0 && (
                <SuccessMessage message='No future events.' />
            )}
            <SuccessMessage message={success} />
            {upcomingGames.length > 0 && <h1>Upcoming games:</h1>}
            <div>
                {upcomingGames.map(game => (
                    <GameCard 
                        key={game.id}
                        game={game}
                    />
                ))}
            </div>
            {feedbackGames.length > 0 && <h1>Required feedbacks:</h1>}
            <div>
                {feedbackGames.map(game => (
                    <FeedbackCard 
                        key={game.id}
                        game={game}
                    />
                ))}
            </div>
            {payments.length > 0 && <h1>Pending payment approvals:</h1>}
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
