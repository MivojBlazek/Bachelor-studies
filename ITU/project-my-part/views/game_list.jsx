import React, { useEffect, useState } from 'react';
import GameCard from '../../Components/delegate/GameCard.jsx';
import axiosClient from '../../axiosClient.js';
import FilterGames from '../../Components/delegate/FilterGames.jsx';
import ErrorMessage from '../../Components/delegate/ErrorMessage.jsx';

export default function GameList() {
    const [upcomingGames, setGames] = useState([]);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchGames = async () => {
            try
            {
                const response = await axiosClient.get('/delegate/games');
                if (Array.isArray(response.data))
                {
                    setGames(response.data);
                    setError(null);
                }
                else
                {
                    setGames([]);
                    setError('No games found.');
                }
            }
            catch (error)
            {
                setGames([]);
                setError('Failed to fetch games.');
            }
        };
    
        fetchGames();
    }, []);
    
    const onFilter = async (params) => {
        try
        {
            const response = await axiosClient.get('/delegate/games', { params: params });
            if (response.data.length > 0)
            {
                setGames(response.data);
                setError(null);
            }
            else
            {
                setGames([]);
                setError('No games found with current filters.');
            }
        }
        catch (error)
        {
            setGames([]);
            setError('Failed to fetch games.');
        }
    }

    return (
        <div style={{margin: '0px 20px', textAlign: 'center'}}>
            <FilterGames onFilter={onFilter} />
            <div>
                <ErrorMessage message={error} />
                {upcomingGames.map(game => (
                    <GameCard 
                        key={game.id}
                        game={game}
                    />
                ))}
            </div>
        </div>
    );
}
