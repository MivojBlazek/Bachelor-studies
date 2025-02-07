/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React, { useState, useEffect } from 'react';
import { useLocation, useNavigate } from 'react-router-dom';
import axiosClient from '../../axiosClient';

export default function PaymentEdit() {
    const location = useLocation();
    const navigate = useNavigate();

    const { game } = location.state;

    const [amount, setAmount] = useState(game.payments.amount || '');

    const handleSubmit = async (e) => {
        e.preventDefault();

        const payload = {
            amount: amount,
            control_id: game.controlId,
        };

        try {
            const response = await axiosClient.post('referee/payments/edit', payload);

            navigate('/referee/payments');
        } catch (error) {
            console.error('Failed to create payment:', error);
        }
    };

    const handleBackClick = () => {
        navigate(-1);
    };

    return (
        <div style={styles.container}>
            <button onClick={handleBackClick} style={styles.backButton}>BACK</button>
            <div style={styles.editsection}>
                <h1 style={styles.title}>HC {game.game.club1} vs. HC {game.game.club2}</h1>
                <div style={styles.line}></div>
                <div style={styles.datacontainer}>
                    <div style={styles.interndatacontainer}><strong>League:</strong> {game.game.league}</div>
                    <div style={styles.interndatacontainer}><strong>Function:</strong> {game.refereeRole}</div>
                    <form onSubmit={handleSubmit}>
                        <div style={styles.interndatacontainer}>
                            <label><strong>Amount (Kč):</strong></label>
                            <input 
                                type="number" 
                                value={amount} 
                                onChange={(e) => setAmount(e.target.value)}
                                style={styles.inputcontainer} 
                            />
                        </div>
                        <button style={styles.backButton} type="submit">Save Changes</button>
                    </form>
                </div>
            </div>
        </div>
    );
}

const styles = {
    backButton: {
        backgroundColor: '#4CAF50',
        color: 'white',
        padding: '10px 20px',
        border: 'none',
        borderRadius: '5px',
        cursor: 'pointer',
        marginBottom: '20px',
        fontSize: '16px',
    },
    line: {
        height: '2px',
        backgroundColor: '#ccc',
        marginBottom: '10px',
        width: '100%',
    },
    editsection: {
        marginTop: '20px',
        fontSize: 'clamp(10px, 2.2vh, 50px)',
        borderTop: '1px solid #ddd',
        padding: '10px',
        width: '100%',
        backgroundColor: '#bdbdbd',
        borderRadius: '8px',
    },
    container: {
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'flex-start',
        justifyContent: 'flex-start',
        fontFamily: 'Arial, sans-serif',
        width: '65vw',
        padding: '20px',
    },
    datacontainer: {
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'flex-start',
        justifyContent: 'flex-start',
        padding: '20px',
        fontSize: 'clamp(10px, 2.5vh, 50px)',
    },
    interndatacontainer: {
        paddingBottom: '20px',
    
    },
    inputcontainer: {
        margin: '0 20px',
        width: '40vw',
        borderRadius: '5px',
        height: '3vh',
        fontSize: 'clamp(10px, 2.5vh, 50px)',
    },
    title: {
        fontSize: 'clamp(10px, 4vh, 50px)',
        fontWeight: 'bold',
        padding:'10px',
        marginBottom: '5px',
        marginTop: '0px',
        textAlign:'center',
    },
}
