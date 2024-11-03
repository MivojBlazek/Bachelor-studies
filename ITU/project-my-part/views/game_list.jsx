import React, { useEffect, useState } from 'react';
import GameCard from '../../Components/delegate/GameCard.jsx';
import axiosClient from '../../axiosClient.js';
import FilterSort from '../../Components/delegate/FilterSort.jsx';

export default function GameList() {
    const [upcomingGames, setGames] = useState([]);

    useEffect(() => {
        const fetchGames = async () => {
            try
            {
                const response = await axiosClient.get('/games');
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
    
    const onFilter = async (params) => {
        try
        {
            const response = await axiosClient.get('/games', { params: params });
            setGames(response.data);
        }
        catch (error)
        {
            setGames([]);
        }
    }

    const onSort = async (params) => {
        try
        {
            const response = await axiosClient.get('/games', { params: { sort: params } });
            setGames(response.data);
        }
        catch (error)
        {
            setGames([]);
        }
    }

    return (
        <div style={{margin: '0px 20px', textAlign: 'center'}}>
            <FilterSort onFilter={onFilter} onSort={onSort} />
            <div>
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
