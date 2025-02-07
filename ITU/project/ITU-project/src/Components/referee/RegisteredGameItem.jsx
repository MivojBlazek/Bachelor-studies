/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import { FaCalendarAlt, FaClock, FaMapMarkerAlt, FaUserTie, FaTrophy } from 'react-icons/fa';
import { BrowserRouter as Router, Routes, Route, Link, useLocation } from 'react-router-dom';
import axiosClient from '../../axiosClient';

export default function RegisteredGameItem({ game, onDelete }) {

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
            backgroundColor: '#ff4d4d',
            color: '#fff',
            border: '1px solid #3190a3',
            borderRadius: '5px',
            cursor: 'pointer',
        },
    };

    const handleDeleteClick = async (e) => {
        e.preventDefault();
    
        try {
            const response = await axiosClient.delete(`/referee/controls`, {
                data: {
                    game_id: game.game.id,
                },
            });
            if (response.status === 200) {
                onDelete(game.game.id);
            }
        } catch (error) {
            console.error('Error deleting control:', error);
        }
    };

    return (
        <Link to={`/referee/all_matches/${game.game.id}/game_detail`} style={{ textDecoration: 'none', color: 'inherit' }}>
        <div style={styles.gameItem}>
            
            <div style={styles.leftColumn}>
                <p style={styles.gameHeader}>HC {game.game.club1.name} vs. HC {game.game.club2.name}</p>
                <p style={styles.gameDetail}> <FaCalendarAlt/> {game.game.date}</p>
                <p style={styles.gameDetail}><FaClock/> {game.game.time}</p>
            </div>
            <div style={styles.rightColumn}>
                <p style={styles.gameDetail}><FaMapMarkerAlt/> {game.game.location}</p>
                <p style={styles.gameDetail}><FaTrophy/> {game.game.league}</p>
                <p style={styles.gameDetail}><FaUserTie/> {game.refereeRole}</p>
            </div>
            

            <div style={styles.ratingColumn}>
                <button style={styles.ratingButton} onClick={handleDeleteClick}>
                    Delete
                </button>
            </div>
        </div>
        </Link>
    );
}