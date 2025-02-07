/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React, { useEffect, useState } from 'react';
import axiosClient from '../../axiosClient';
import GameItem from './GameItem';

export default function Dashboar() {
    const [games, setGames] = useState([]);
    const [loading, setLoading] = useState(true);
    const [todayGames, setTodayGames] = useState([]);
    const [tomorrowGames, setTomorrowGames] = useState([]);
    const [futureGames, setFutureGames] = useState([]);
    
    useEffect(() => {
        const fetchGames = async () => {
            try {
                const response = await axiosClient.get('/referee/dashboard');
                const allGames = response.data;
                const today = new Date().toISOString().split('T')[0];
                const tomorrow = new Date(Date.now() + 86400000).toISOString().split('T')[0];

                const todayGamesList = [];
                const tomorrowGamesList = [];
                const futureGamesList = [];

                allGames.forEach((game) => {
                    const gameDate = game.game.date;

                    if (gameDate === today) {
                        todayGamesList.push(game);
                    } else if (gameDate === tomorrow) {
                        tomorrowGamesList.push(game);
                    } else if (new Date(gameDate) > new Date(tomorrow)) {
                        futureGamesList.push(game);
                    }
                });

                setTodayGames(todayGamesList);
                setTomorrowGames(tomorrowGamesList);
                setFutureGames(futureGamesList);
                setGames(allGames);
                setLoading(false);
            } catch (error) {
                console.error('Chyba při načítání zápasů:', error.response || error.message);
                setLoading(false);
            }
        };

        fetchGames();
    }, []);

    if (loading) {
        return <div>Loading...</div>;
    }


    return (
        <div style={styles.dashboardContainer}>
            <h1 style={styles.mainTitle}>Upcoming events</h1>
            <div style={styles.line}></div>

            <div>
                <h2 style={styles.eventTitle}>Games Today</h2>
                {todayGames.length > 0 ? (
                    todayGames.map((game, index) => <GameItem key={index} game={game} />)
                ) : (
                    <p style={styles.noEvent}>No games today.</p>
                )}

                <h2 style={styles.eventTitle}>Games Tomorrow</h2>
                {tomorrowGames.length > 0 ? (
                    tomorrowGames.map((game, index) => <GameItem key={index} game={game} />)
                ) : (
                    <p style={styles.noEvent}>No games tomorrow.</p>
                )}

                <h2 style={styles.eventTitle}>Upcoming Games</h2>
                {futureGames.length > 0 ? (
                    futureGames.map((game, index) => <GameItem key={index} game={game} />)
                ) : (
                    <p style={styles.noEvent}>No upcoming games.</p>
                )}
            </div>
            
        </div>
    );
}

const styles = {
    dashboardContainer: {
        width: '55vw',
        padding: '20px',
        paddingLeft: '3vw',
        fontFamily: 'Arial, sans-serif',
        border: '3px solid #a6a6a6',
        backgroundColor: '#bdbdbd',
        borderRadius: '8px',
        boxShadow: '0 4px 8px rgba(0, 0, 0, 0.1)',
        display: 'flex',
        justifyContent: 'flex-start',
        alignItems: 'flex-start',
        flexDirection: 'column', 
    },
    mainTitle: {
        textAlign: 'center',
        fontSize: 'clamp(10px, 3.2vh, 50px)',
        width: '100%',
        marginBottom: '5px',
    },
    line: {
        height: '2px',
        backgroundColor: '#ccc',
        marginBottom: '10px',
        width: '100%',
    },
    eventTitle: {
        fontSize: 'clamp(10px, 2vh, 40px)',
        fontWeight: 'bold',
        marginTop: '15px',
        color: '#333',
    },
    noEvent: {
        fontSize: 'clamp(8px, 1.9vh, 35px)',
        padding: '0 10px',
        color: '#222'
    },
};
