import React, { useEffect, useState } from 'react';
import { useParams } from 'react-router-dom';
import axiosClient from '../../axiosClient';
import RefereeList from '../../Components/delegate/RefereeList';
import Button from '../../Components/delegate/Button';
import P from '../../Components/delegate/P';

export default function GameDetail() {
    const { id } = useParams();
    const [game, setGame] = useState(null);
    const [feedbacks, setFeedbacks] = useState({});
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchGame = async () => {
            try
            {
                const response = await axiosClient.get(`/games/${id}`);
                setGame(response.data);

                const initialFeedbacks = {};
                response.data.controls.forEach(control => {
                    initialFeedbacks[control.id] = control.feedback || '';
                });
                setFeedbacks(initialFeedbacks);
            }
            catch (error)
            {
                setError(error.message);
            }
        };

        fetchGame();
    }, [id]);

    if (!game)
    {
        return null;
    }

    const openVideos = () => {
        //TODO
    }

    const addVideo = () => {
        //TODO
    }

    return (
        <div style={{ textAlign: 'center' }}>
            <h1>Game Details</h1>
            <h2>
                <P label={`${game.club1.name}`} href='/delegate/dashboard' />{/*//TODO*/}
                &nbsp;vs&nbsp;
                <P label={`${game.club2.name}`} href='/delegate/dashboard' />{/*//TODO*/}
            </h2>
            <p>League: {game.league}</p>
            <p>Location: {game.location}</p>
            <p>Date: {game.date}</p>
            <p>Time: {game.time}</p>
            <P label={`Delegate: ${game.delegate.name}`} href='/delegate/dashboard' />{/*//TODO*/}
            <RefereeList    
                game={game}
                feedbacks={feedbacks}
                setFeedbacks={setFeedbacks}
                error={error}
                setError={setError}
            />
            <Button
                label='Videos'
                onClick={openVideos}
            />
            <Button
                label='Add video'
                onClick={addVideo}
            />
        </div>
    );
}