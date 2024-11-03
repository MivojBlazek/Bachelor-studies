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
            <p>Game date: {game.date}</p>
            <p>Game location: {game.location}</p>
            <p>Referees: {game.refereesWithoutFeedback.join(', ')}</p>
        </>
    );

    return (
        <div>
            <Card onClick={goToDetail} content={cardContent} />
        </div>
    );
}
