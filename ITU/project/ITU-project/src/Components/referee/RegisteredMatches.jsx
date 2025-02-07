/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React, { useEffect, useState } from 'react';
import axiosClient from '../../axiosClient';
import RegisteredGameItem from './RegisteredGameItem';
import Filters from './Filter';


export default function RegisteredMatches() {
    const [matches, setMatches] = useState([]);
    const [filteredGames, setFilteredGames] = useState([]);
    const [filters, setFilters] = useState({
        date: '',
        location: '',
        club: '',
        role: '',
        league: '',
    });
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchRegisteredMatches = async () => {
            try {
                const response = await axiosClient.get('/referee/registered_matches');
                const sortedGames = response.data.sort((a, b) =>
                new Date(a.game.date) - new Date(b.game.date)
                );
                setMatches(sortedGames);
                setFilteredGames(sortedGames);
                setLoading(false);
            } catch (err) {
                setError(err.response ? err.response.data.message : 'Error fetching data');
                setLoading(false);
            }
        };

        fetchRegisteredMatches();
    }, []);

    if (loading) {
        return <div>Loading...</div>;
    }

    const handleFilterChange = (e) => {
        const { name, value } = e.target;
        const newFilters = { ...filters, [name]: value };

        setFilters(newFilters);

        const filtered = matches.filter((game) => {
            return (
                (!newFilters.date || game.game.date.includes(newFilters.date)) &&
                (!newFilters.location ||
                    game.game.location.toLowerCase().includes(newFilters.location.toLowerCase())) &&
                (!newFilters.club ||
                    game.game.club1.name.toLowerCase().includes(newFilters.club.toLowerCase()) ||
                    game.game.club2.name.toLowerCase().includes(newFilters.club.toLowerCase())) &&
                (!newFilters.role || game.refereeRole.toLowerCase().includes(newFilters.role.toLowerCase())) &&
                (!newFilters.league ||
                    game.game.league.toLowerCase().includes(newFilters.league.toLowerCase()))
            );
        });

        setFilteredGames(filtered);
    };

    const gamesByDate = filteredGames.reduce((grouped, game) => {
        const gameDate = game.game.date;
        if (!grouped[gameDate]) {
            grouped[gameDate] = [];
        }
        grouped[gameDate].push(game);
        return grouped;
    }, {});

    const handleRemoveGame = (gameId) => {
        setMatches((prevMatches) => prevMatches.filter((game) => game.game.id !== gameId));
        setFilteredGames((prevFilteredGames) =>
            prevFilteredGames.filter((game) => game.game.id !== gameId)
        );
    };

    return (
        <div style={styles.container}>
            <Filters filters={filters} onFilterChange={handleFilterChange} />
            <div style={styles.line}></div>
            {Object.entries(gamesByDate).length > 0 ? (
                Object.entries(gamesByDate).map(([date, gamesForDate]) => (
                    <div key={date}>
                        <h2 style={styles.dateTitle}>{date}</h2>
                        {gamesForDate.map((game, index) => (
                            <RegisteredGameItem key={index} game={game} onDelete={handleRemoveGame}/>
                        ))}
                    </div>
                ))
            ) : (
                <p>No matches found</p>
            )}
        </div>
    );
}

const styles = {
    container: {
        width: '65vw',
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
    line: {
        height: '2px',
        backgroundColor: '#ccc',
        marginBottom: '20px',
        width: '100%',
    },
    dateTitle: {
        fontSize: 'clamp(12px, 2.5vh, 30px)',
        fontWeight: 'bold',
        marginBottom: '5px',
        color: '#333',
    },
};