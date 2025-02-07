/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import { FaCalendarAlt, FaClock, FaMapMarkerAlt, FaUserTie, FaTrophy } from 'react-icons/fa';
import { useNavigate } from 'react-router-dom';
import { BrowserRouter as Router, Routes, Route, Link, useLocation } from 'react-router-dom';

export default function PaymentItem({ game }) {
    const navigate = useNavigate();

    const styles = {
        gameItem: {
            width:'60vw',
            fontFamily: 'Arial, sans-serif',
            display: 'flex',
            padding: '15px',
            marginBottom: '10px',
            backgroundColor: '#62d8f0',
            borderRadius: '8px',
            border: '3px solid #3bb4cc',
            justifyContent: 'space-between',
        },
        gameHeader: {
            fontWeight: 'bold',
            color: '#666',
            fontSize: 'clamp(10px, 2vh, 40px)',
            margin: '0px',
            marginBottom: '8px',
        },
        gameDetail: {
            color: '#666',
            fontSize: 'clamp(9px, 1.9vh, 25px)',
            margin: '0px',
            marginBottom: '2px',
        },
        leftColumn: {
            display: 'flex',
            flexDirection: 'column',
            justifyContent: 'space-between',
            width: '40%',
        },
        rightColumn: {
            display: 'flex',
            flexDirection: 'column',
            justifyContent: 'space-between',
            width: '25%',
        },
        ratingColumn: {
            display: 'flex',
            justifyContent: 'center',
            alignItems: 'center',
            width: '20%',
        },
        ratingButton: {
            padding: '10px 28px',
            fontSize: 'clamp(9px, 2.5vh, 20px)',
            backgroundColor: '#3bb4cc',
            color: '#fff',
            border: '1px solid #3190a3',
            borderRadius: '5px',
            cursor: 'pointer',
        },
        amountText: {
            fontSize: 'clamp(9px, 2.5vh, 20px)',
            color: '#fff',
            backgroundColor: '#3bb4cc',
            padding: '10px 15px',
            borderRadius: '5px',
            textAlign: 'center',
        },
    };

    const handleRatingClick = () => {
        navigate(`/referee/payments/${game.controlId}/payment_edit`, { state: { game: game } });
    };

    return (
        <Link to={`/referee/all_matches/${game.game.id}/game_detail`} style={{ textDecoration: 'none', color: 'inherit' }}>
        <div style={styles.gameItem}>
            
            <div style={styles.leftColumn}>
                <p style={styles.gameHeader}>HC {game.game.club1} vs. HC {game.game.club2}</p>
                <p style={styles.gameDetail}><FaCalendarAlt/> {game.game.date}</p>
                <p style={styles.gameDetail}><FaClock/> {game.game.time}</p>
                <p style={styles.gameDetail}><FaMapMarkerAlt/> {game.game.location}</p>
            </div>
            <div style={styles.rightColumn}>
                
                <p style={styles.gameDetail}><FaTrophy/> {game.game.league}</p>
                <p style={styles.gameDetail}><FaUserTie/> {game.refereeRole}</p>
                <p style={styles.gameDetail}><FaUserTie/> {game.payments.approvedBy}</p>
                <p style={styles.gameDetail}><FaCalendarAlt/> {game.payments.approvalDate}</p>
            </div>
            

            <div style={styles.ratingColumn}>
                {game.payments.amount ? (
                        <div style={styles.amountText}>{game.payments.amount} Kč</div>
                    ) : (
                        <button
                            style={styles.ratingButton}
                            onClick={(e) => {
                                e.preventDefault();
                                handleRatingClick();
                            }}
                        >
                            Edit
                        </button>
                    )}
            </div>
        </div>
        </Link>
    );
}