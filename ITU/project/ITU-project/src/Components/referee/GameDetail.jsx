/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React, { useState, useEffect } from 'react';
import { useParams } from 'react-router-dom';
import { useNavigate } from 'react-router-dom';
import axiosClient from '../../axiosClient';
import { FaCalendarAlt, FaClock, FaMapMarkerAlt, FaUserTie, FaTrophy } from 'react-icons/fa';

export default function GameDetail() {
    const { id } = useParams();
    const [game, setGame] = useState(null);
    const [error, setError] = useState(null);
    const navigate = useNavigate();

    useEffect(() => {
        axiosClient.get(`/referee/game_detail/${id}`)
            .then(response => setGame(response.data))
            .catch(err => setError(err.message));
    }, [id]);

    if (error) {
        return <p>Chyba: {error}</p>;
    }

    if (!game) {
        return <p>Loading...</p>;
    }

    const handleRatingClick = () => {
        navigate(`/referee/all_matches/videos`, { state: { videos: game.videos } });
    };

    return (
        <div style={styles.container}>
            <div style={styles.header}>
                <div style={styles.header1}>
                    <img 
                            src={game.club1.photo} 
                            alt={"Profile picture"} 
                            style={styles.profileImage} 
                    />
                    <h1>HC {game.club1.name} vs. HC {game.club2.name}</h1>
                    <img 
                        src={game.club2.photo} 
                        alt={"Profile picture"} 
                        style={styles.profileImage} 
                    />
                </div>
                
                <p><FaMapMarkerAlt/> {game.location}</p>
            </div>
            <div style={styles.line}></div>
            <div style={styles.delegate}>
                <ul style={styles.ul}>
                    {game.controls && game.controls?.length > 0 ? (
                        game.controls.map(control => (
                            control.referee ? (
                                <li key={control.referee.id}>
                                    <FaUserTie/> <strong>{control.refereeRole}:</strong> {control.referee.name}
                                </li>
                            ) : (
                                <li>Rozhodčí nejsou k dispozici</li>
                            )
                        ))
                    ) : (
                        <li>Žádní rozhodčí k dispozici</li>
                    )}
                </ul>
                <p><strong><FaUserTie/> Delegate:</strong> {game.delegate?.name}</p>
            </div>
            <div style={styles.clubs}>
                <div style={styles.clubdetail}>
                    <h2 style={styles.headerh2}>HC {game.club1.name}</h2>
                    <div style={styles.line}></div>
                    <ul style={styles.ul}>
                        {game.club1.players.map(player => (
                            <li key={player.id}>{player.name} - {player.role} {player.dress_number}</li>
                        ))}
                    </ul>
                </div>
                <div style={styles.clubdetail}>
                    <h2 style={styles.headerh2}>HC {game.club2.name}</h2>
                    <div style={styles.line}></div>
                    <ul style={styles.ul}>
                        {game.club2.players.map(player => (
                            <li key={player.id}>{player.name} - {player.role} {player.dress_number}</li>
                        ))}
                    </ul>
                </div>
            </div>
            <div style={styles.gameInfo}>
                <button style={styles.videoButton} onClick={(e) => {e.preventDefault(); handleRatingClick();}}>
                    Videos
                </button>
            </div>
        </div>
    );
}

const styles = {
    container: {
        display: 'flex',
        justifyContent: 'flex-start',
        alignItems: 'center',
        flexDirection: 'column', 
        fontFamily: 'Arial, sans-serif',
        width:'65vw',
        padding: '10px',
        backgroundColor: '#bdbdbd',
        borderRadius: '10px',
        margin: '20px auto',
    },
    header: {
        display: 'flex',
        alignItems: 'center',
        flexDirection: 'column',
        fontSize: 'clamp(10px, 2vh, 40px)',
        marginBottom: '0px',
    },
    line: {
        height: '2px',
        backgroundColor: '#ccc',
        marginBottom: '20px',
        width: '100%',
    },
    headerh2: {
        fontSize: 'clamp(10px, 2.5vh, 40px)',
        textAlign: 'center',
    },
    delegate: {
        width:'90%',
        display: 'flex',
        padding: '10px',
        fontSize: 'clamp(10px, 2.2vh, 40px)',
        alignItems: 'flex-start',
        justifyContent: 'flex-start',
        flexDirection: 'column', 
        marginBottom: '10px',
        backgroundColor: '#62d8f0',
        border: '3px solid #3bb4cc',
        borderRadius: '10px',
    },
    header1: {
        display: 'flex',
        alignItems: 'center',
        flexDirection: 'row',
    },
    clubs: {
        width:'92%',
        display: 'flex',
        padding: '0px',
        fontSize: 'clamp(10px, 2vh, 40px)',
        alignItems: 'stretch',
        justifyContent: 'space-between',
        flexDirection: 'row', 
        marginBottom: '10px',
        
    },
    clubdetail: {
        backgroundColor: '#62d8f0',
        border: '3px solid #3bb4cc',
        borderRadius: '10px',
        padding: '10px',
        width: '45%',
        display: 'flex',
        flexDirection: 'column',
        
    },
    gameInfo: {
        marginTop: '10px',
    },
    ul: {
        listStyleType: 'none',
        paddingLeft: '0',
        marginBottom: '0',
        marginTop: '0',       
    },
    profileImage: {
        height: 'clamp(10px, 7vh, 55px)',
        width: 'clamp(10px, 7vh, 55px)',
        borderRadius: '50%',
        objectFit: 'cover',
        fontSize: 'clamp(5px, 1vh, 10px)',
        fontWeight: 'normal',
        padding: '0px 20px'
    },
    videoButton: {
        padding: '10px 28px',
        fontSize: 'clamp(9px, 2.5vh, 20px)',
        backgroundColor: '#3bb4cc',
        color: '#fff',
        border: '1px solid #3190a3',
        borderRadius: '5px',
        cursor: 'pointer',
    },
};
