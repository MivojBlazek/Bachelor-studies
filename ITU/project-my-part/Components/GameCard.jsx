/**
 * ITU project
 * 
 * File: GameCard.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React from 'react';
import { useNavigate } from 'react-router-dom';
import { MdSportsHockey } from "react-icons/md";
import Card from './Card.jsx';

export default function GameCard({ game }) {
    const navigate = useNavigate();

    const goToDetail = (ev) => {
        ev.preventDefault();
        navigate(`/delegate/game_detail/${game.id}`);
    };

    const cardContent = (
        <>
            <h2
                style={{
                    display: 'grid',
                    gridTemplateColumns: '1fr auto 1fr',
                    gap: '10px',
                    margin: '0 auto',
                }}
            >
                <span style={{ textAlign: 'right' }}>{game.club1.name}</span>
                <MdSportsHockey style={{ fontSize: '30px' }} />
                <span style={{ textAlign: 'left' }}>{game.club2.name}</span>
            </h2>
            <div
                style={{
                    margin: '0 auto',
                    width: 'fit-content',
                    display: 'grid',
                    gridTemplateColumns: '1fr 1fr',
                    gap: '13px',
                    textAlign: 'left',
                    padding: '15px',
                }}
            >
                <span style={{ textAlign: 'right' }}>League:</span><span>{game.league}</span>
                <span style={{ textAlign: 'right' }}>Location:</span><span>{game.location}</span>
                <span style={{ textAlign: 'right' }}>Date:</span><span>{(new Date(game.date)).toLocaleDateString()}</span>
                <span style={{ textAlign: 'right' }}>Time:</span><span>{game.time}</span>
            </div>
        </>
    );

    return (
        <div>
            <Card onClick={goToDetail} content={cardContent} />
        </div>
    );
}
