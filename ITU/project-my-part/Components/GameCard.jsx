import React from 'react';
import { useNavigate } from 'react-router-dom';
import Card from './Card.jsx';

export default function GameCard({ game }) {
    const navigate = useNavigate();

    const goToDetail = (ev) => {
        ev.preventDefault();
        navigate(`/delegate/game_detail/${game.id}`);
    };

    const cardContent = (
        <>
            <h2>{game.club1.name} vs {game.club2.name}</h2>
            <p>League: {game.league}</p>
            <p>Location: {game.location}</p>
            <p>Date: {game.date}</p>
            <p>Time: {game.time}</p>
        </>
    );

    return (
        <div>
            <Card onClick={goToDetail} content={cardContent} />
        </div>
    );
}
