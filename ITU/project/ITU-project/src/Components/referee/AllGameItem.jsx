/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React , { useState }from 'react';
import { FaCalendarAlt, FaClock, FaMapMarkerAlt, FaUserTie, FaTrophy } from 'react-icons/fa';
import { BrowserRouter as Router, Routes, Route, Link, useLocation } from 'react-router-dom';
import axiosClient from '../../axiosClient';

export default function AllGameItem({ game, onUpdateGame}) {
    const [gameData, setGameData] = useState({
        ...game,
        controls: game.controls || [],
    });

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
            width: '35%',
        },
        rightColumn: {
            display: 'flex',
            flexDirection: 'column',
            justifyContent: 'space-between',
            width: '30%',
        },
        ratingColumn: {
            display: 'flex',
            flexDirection: 'column',
            justifyContent: 'space-between',
            alignItems: 'center',
            width: '20%',
            gap: '10px',
        },
        mainButton: {
            padding: '10px 28px',
            fontSize: 'clamp(9px, 2vh, 20px)',
            backgroundColor: '#3bb4cc',
            color: '#fff',
            border: '1px solid #3190a3',
            borderRadius: '5px',
            cursor: 'pointer',
        },
        lineButton: {
            padding: '10px 28px',
            fontSize: 'clamp(9px, 2vh, 20px)',
            backgroundColor: '#3bb4cc',
            color: '#fff',
            border: '1px solid #3190a3',
            borderRadius: '5px',
            cursor: 'pointer',
        },
    };
    const handleAddMainReferee = async () => {
        const controlData = {
            refereeRole: 'main',
            game_id: gameData.id,
        };
    
        try {
            const response = await axiosClient.post('/referee/controls/add', controlData);
    
            setGameData((prevGameData) => {
                const updatedControls = [...prevGameData.controls, response.data];
                const updatedGame = { ...prevGameData, controls: updatedControls };
    
                onUpdateGame(updatedGame);
    
                return updatedGame;
            });
    
        } catch (error) {
            console.error('Error adding main referee:', error);
        }
    };
    
    const handleAddLineReferee = async () => {
        const lineReferees = gameData.controls.filter(control => control.refereeRole.toLowerCase() === 'line');
        if (lineReferees.length >= 2) {
            return;
        }
    
        const controlData = {
            refereeRole: 'line',
            game_id: gameData.id,
        };
    
        try {
            const response = await axiosClient.post('/referee/controls/add', controlData);
    
            setGameData((prevGameData) => {
                const updatedControls = [...prevGameData.controls, response.data];
                const updatedGame = { ...prevGameData, controls: updatedControls };
    
                onUpdateGame(updatedGame);

                return updatedGame;
            });
    
        } catch (error) {
            console.error('Error adding line referee:', error);
        }
    };

    const getRefereesByRole = (role) => {
        const referees = gameData.controls.filter((control) => control?.refereeRole?.toLowerCase() === role.toLowerCase());
        
        if (role === 'main') {
            return referees.length > 0 ? referees[0]?.referee?.name : 'No referee found';
        }

        if (role === 'line') {
            return referees.slice(0, 2).map((referee, index) => `${referee?.referee?.name}`).join(', ') || 'No referee found';
        }

        return 'No referee found';
    };

    const mainRefereeAssigned = gameData.controls.some((control) => control?.refereeRole?.toLowerCase() === 'main' && control.referee_id != null);

    const lineReferees = gameData.controls.filter(control => control.refereeRole?.toLowerCase() === 'line');
    const line1Assigned = lineReferees.length > 0;
    const line2Assigned = lineReferees.length > 1;

    const isFutureOrTodayGame = () => {
        const gameDate = new Date(gameData.date);
        const currentDate = new Date();
        return gameDate >= currentDate;
    };

    return (
        <Link to={`/referee/all_matches/${game.id}/game_detail`} style={{ textDecoration: 'none', color: 'inherit' }}>
        <div key={gameData.controls.length} style={styles.gameItem}>
            
            <div style={styles.leftColumn}>
                <p style={styles.gameHeader}>HC {game.club1.name} vs. HC {game.club2.name}</p>
                <p style={styles.gameDetail}> <FaCalendarAlt/> {game.date}</p>
                <p style={styles.gameDetail}><FaClock/> {game.time}</p>
                <p style={styles.gameDetail}><FaMapMarkerAlt/> {game.location}</p>
            </div>
            <div style={styles.rightColumn}>
                <p style={styles.gameDetail}><FaTrophy/> {game.league}</p>
                <p style={styles.gameDetail}><FaUserTie/> MAIN: {getRefereesByRole('main')}</p>
                <p style={styles.gameDetail}><FaUserTie/> LINE1: {getRefereesByRole('line').split(', ')[0]}</p>
                <p style={styles.gameDetail}><FaUserTie/> LINE2: {getRefereesByRole('line').split(', ')[1] || 'No referee found'}</p>
            </div>
            

            <div style={styles.ratingColumn}>
                    {isFutureOrTodayGame() && !mainRefereeAssigned && (
                        <button style={styles.mainButton} onClick={(e) => { e.preventDefault(); handleAddMainReferee(); }}>
                            ADD Main
                        </button>
                    )}
                    {isFutureOrTodayGame() && !(line1Assigned && line2Assigned) && (
                        <button style={styles.lineButton} onClick={(e) => { e.preventDefault(); handleAddLineReferee(); }}>
                            ADD Line
                        </button>
                    )}
            </div>
        </div>
        </Link>
    );
}