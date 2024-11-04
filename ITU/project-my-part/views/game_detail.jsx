import React, { useEffect, useState } from 'react';
import { useParams, useNavigate } from 'react-router-dom';
import axiosClient from '../../axiosClient';
import RefereeList from '../../Components/delegate/RefereeList';
import Button from '../../Components/delegate/Button';
import ButtonSuccess from '../../Components/delegate/ButtonSuccess';
import ButtonFailure from '../../Components/delegate/ButtonFailure';
import P from '../../Components/delegate/P';
import InputWithLabel from '../../Components/delegate/InputWithLabel';
import TextareaWithLabel from '../../Components/delegate/TextareaWithLabel';
import ErrorMessage from '../../Components/delegate/ErrorMessage';
import { useStateContext } from '../../contexts/contextprovider';

export default function GameDetail() {
    const { id } = useParams();
    const [game, setGame] = useState(null);
    const [feedbacks, setFeedbacks] = useState({});
    const [error, setError] = useState(null);
    const [showVideoForm, setShowVideoForm] = useState(false);
    const [url, setUrl] = useState('');
    const [description, setDescription] = useState('');
    const navigate = useNavigate();
    const { user } = useStateContext();

    useEffect(() => {
        const fetchGame = async () => {
            try
            {
                const response = await axiosClient.get(`/delegate/games/${id}`);
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
        navigate(`/delegate/game/${id}/videos`);
    }

    const addVideo = async () => {
        if (showVideoForm)
        {
            if (url)
            {
                try
                {
                    await axiosClient.post(`/delegate/videos`, { url, description, id });

                    setUrl('');
                    setDescription('');
                    setShowVideoForm(false);
                    setError(null);
                }
                catch (error)
                {
                    if (error.response && error.response.status === 422)
                    {
                        const validationError = error.response.data.errors?.url?.[0];
                        setError(validationError);
                    }
                    else
                    {
                        setError(error.message);
                    }
                }
            }
            else
            {
                setError('URL required!');
            }
        }
        else
        {
            setShowVideoForm(true);
        }
    }

    const signUp = async (id) => {
        try
        {
            await axiosClient.post(`/delegate/game/${id}/signUp`);
            window.location.reload();
            setError(null);
        }
        catch (error)
        {
            setError(error.response?.data?.message || error.message);
        }
    }

    const signOut = async (id) => {
        try
        {
            await axiosClient.post(`/delegate/game/${id}/signOut`);
            window.location.reload();
            setError(null);
        }
        catch (error)
        {
            setError(error.response?.data?.message || error.message);
        }
    }

    return (
        <div style={{ textAlign: 'center' }}>
            <h1>Game Details</h1>
            <h2>
                <P label={`${game.club1.name}`} href={`/delegate/club_profile/${game.club1.id}`} />
                &nbsp;vs&nbsp;
                <P label={`${game.club2.name}`} href={`/delegate/club_profile/${game.club2.id}`} />
            </h2>
            <p>League: {game.league}</p>
            <p>Location: {game.location}</p>
            <p>Date: {game.date}</p>
            <p>Time: {game.time}</p>
            <div
                style={{
                    display: 'flex',
                    alignSelf: 'center',
                    justifyContent: 'center',
                    margin: '0px auto',
                    gap: '8px',
                }}
            >
                <p style={{ fontWeight: 'bold', margin: '5px' }}>Delegate:</p>
                {game.delegate !== null ? ( 
                    <>
                        <P
                            label={`${game.delegate.name}`}
                            href={`/delegate/delegate_profile/${game.delegate.id}`}
                            style={{
                                margin: '5px',
                            }}
                        />
                        {(game.delegate_id === user.id) && (new Date(game.date) > new Date()) && (
                            <ButtonFailure
                                label='Sign out'
                                onClick={() => signOut(id)}
                                style={{
                                    padding: '5px 15px',
                                    margin: '0px',
                                }}
                            />
                        )}
                    </>
                ) : (
                    new Date(game.date) > new Date() ? (
                        <ButtonSuccess
                            label='Sign up'
                            onClick={() => signUp(id)}
                            style={{
                                padding: '5px 15px',
                                margin: '0px',
                            }}
                        />
                    ) : (<p style={{ margin: '5px' }}>---</p>)
                )}
            </div>
            <RefereeList    
                game={game}
                feedbacks={feedbacks}
                setFeedbacks={setFeedbacks}
                error={error}
                setError={setError}
            />
            <ErrorMessage message={error}/>
            <Button
                style={{
                    marginBottom: '30px'
                }}
                label='Videos'
                onClick={openVideos}
            />
            {showVideoForm && (
                <form onSubmit={(e) => e.preventDefault()}>
                    <div
                        style={{
                            alignSelf: 'center',
                            justifyContent: 'center',
                            margin: '0px auto',
                            maxWidth: '800px',
                            display: 'flex',
                            gap: '20px',
                        }}
                    >
                        <InputWithLabel
                            label='URL:'
                            type='url'
                            value={url}
                            onChange={(e) => setUrl(e.target.value)}
                            required
                        />
                        <TextareaWithLabel
                            label='Description:'
                            value={description}
                            onChange={(e) => setDescription(e.target.value)}
                        />
                    </div>
                </form>
            )}
            <Button
                label='Add video'
                onClick={addVideo}
            />
        </div>
    );
}