/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import { FaCalendarAlt, FaClock, FaMapMarkerAlt, FaUserTie, FaTrophy } from 'react-icons/fa';

export default function GameItem({ game }) {
    const styles = {
        gameItem: {
            width:'50vw',
            fontFamily: 'Arial, sans-serif',
            display: 'flex',
            padding: '15px',
            marginBottom: '20px',
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
            width: '45%',
        },
        rightColumn: {
            display: 'flex',
            flexDirection: 'column',
            justifyContent: 'space-between',
            width: '45%',
        },
    };

    return (
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
        </div>
    );
}