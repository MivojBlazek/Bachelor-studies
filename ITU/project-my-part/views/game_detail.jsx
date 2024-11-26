import React, { useEffect, useState } from 'react';
import { useParams, useNavigate } from 'react-router-dom';
import { MdOutlineDoNotDisturbOnTotalSilence, MdSportsHockey } from "react-icons/md";
import axiosClient from '../../axiosClient';
import RefereeList from '../../Components/delegate/RefereeList';
import Button from '../../Components/delegate/Button';
import ButtonSuccess from '../../Components/delegate/ButtonSuccess';
import ButtonFailure from '../../Components/delegate/ButtonFailure';
import P from '../../Components/delegate/P';
import InputWithLabel from '../../Components/delegate/InputWithLabel';
import TextareaWithLabel from '../../Components/delegate/TextareaWithLabel';
import ErrorMessage from '../../Components/delegate/ErrorMessage';
import SuccessMessage from '../../Components/delegate/SuccessMessage';
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
    const [success, setSuccess] = useState(null);

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
                setError(null);
            }
            catch (error)
            {
                if (error.response)
                {
                    setError(error.response.data.error);
                }
                else
                {
                    setError(error.message);
                }
                setSuccess(null);
            }
        };

        fetchGame();
    }, [id]);

    if (!game && !error)
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
                    const response = await axiosClient.post(`/delegate/videos`, { url, description, id });

                    setUrl('');
                    setDescription('');
                    setShowVideoForm(false);
                    setError(null);
                    setSuccess(response.data.success);
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
            const response = await axiosClient.post(`/delegate/game/${id}/signUp`);
            setSuccess(null);
            if (response.data.success)
            {
                setSuccess(response.data.success);
                setGame((prev) => ({
                    ...prev,
                    delegate: { id: user.id, name: user.name },
                    delegate_id: user.id,
                }));
            }
            setError(null);
        }
        catch (error)
        {
            setSuccess(null);
            setError(error.response?.data?.error || error.message);
        }
    }

    const signOut = async (id) => {
        try
        {
            const response = await axiosClient.post(`/delegate/game/${id}/signOut`);
            setSuccess(null);
            if (response.data.success)
            {
                setSuccess(response.data.success);
                setGame((prev) => ({
                    ...prev,
                    delegate: null,
                    delegate_id: null,
                }));
            }
            setError(null);
        }
        catch (error)
        {
            setSuccess(null);
            setError(error.response?.data?.error || error.message);
        }
    }

    return (
        <>
            {!game && <ErrorMessage message={error} />}
            {game && (
                <div style={{ textAlign: 'center' }}>
                    <h1
                        style={{
                            display: 'grid',
                            gridTemplateColumns: '1fr auto 1fr',
                            gap: '15px',
                        }}
                    >
                        <P label={`${game.club1.name}`} href={`/delegate/club_profile/${game.club1.id}`} style={{ textAlign: 'right' }}/>
                        <MdSportsHockey style={{ fontSize: '42px' }} />
                        <P label={`${game.club2.name}`} href={`/delegate/club_profile/${game.club2.id}`} style={{ textAlign: 'left' }}/>
                    </h1>
                    <div
                        style={{
                            margin: '0 auto',
                            width: 'fit-content',
                            fontWeight: 'bold',
                            display: 'grid',
                            gridTemplateColumns: '1fr 1fr',
                            gap: '13px',
                            textAlign: 'left',
                        }}
                    >
                        <p style={{ margin: '0px', textAlign: 'right' }}>League:</p><p style={{ margin: '0px' }}>{game.league}</p>
                        <p style={{ margin: '0px', textAlign: 'right' }}>Location:</p><p style={{ margin: '0px' }}>{game.location}</p>
                        <p style={{ margin: '0px', textAlign: 'right' }}>Date:</p><p style={{ margin: '0px' }}>{(new Date(game.date)).toLocaleDateString()}</p>
                        <p style={{ margin: '0px', textAlign: 'right' }}>Time:</p><p style={{ margin: '0px' }}>{game.time}</p>
                        <p style={{ margin: '0px', textAlign: 'right', padding: '6px 0px' }}>Delegate:</p>
                        {game.delegate !== null ? (
                            ((game.delegate_id === user.id) && (new Date(game.date) > new Date())) ? (
                                <span style={{ padding: '3px 0px' }}>
                                    <P
                                        label={`${game.delegate.name}`}
                                        href={`/delegate/delegate_profile/${game.delegate.id}`}
                                    />
                                    <ButtonFailure
                                        label='Sign out'
                                        onClick={() => signOut(id)}
                                        style={{
                                            padding: '6px 15px',
                                            margin: '0px',
                                            marginLeft: '13px',
                                        }}
                                    />
                                </span>
                            ) : (
                                <P
                                    label={`${game.delegate.name}`}
                                    href={`/delegate/delegate_profile/${game.delegate.id}`}
                                    style={{
                                        padding: '6px 0px',
                                    }}
                                />
                            )
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
                            ) : (<p style={{ margin: '5px 0px' }}>---</p>)
                        )}
                    </div>
                    <RefereeList
                        game={game}
                        feedbacks={feedbacks}
                        setFeedbacks={setFeedbacks}
                        error={error}
                        setError={setError}
                        isMe={(game.delegate_id === user.id) && (new Date(game.date) < new Date())}
                    />
                    <SuccessMessage message={success}/>
                    <ErrorMessage message={error}/>
                    <Button
                        style={{
                            marginBottom: '30px'
                        }}
                        label='Videos'
                        onClick={openVideos}
                    />
                    {showVideoForm && (
                        <form onSubmit={(e) => e.preventDefault()} style={{ margin: '10px' }}>
                            <div
                                style={{
                                    alignSelf: 'center',
                                    justifyContent: 'center',
                                    margin: '10px',
                                    display: 'flex',
                                    flexDirection: 'row',
                                    flexWrap: 'wrap',
                                    gap: '20px',
                                }}
                            >
                                <InputWithLabel
                                    placeholder='Insert video URL...'
                                    label='URL:'
                                    type='url'
                                    value={url}
                                    onChange={(e) => setUrl(e.target.value)}
                                    required
                                />
                                <TextareaWithLabel
                                    placeholder='Insert description... (Optional)'
                                    label='Description:'
                                    value={description}
                                    onChange={(e) => setDescription(e.target.value)}
                                />
                            </div>
                        </form>
                    )}
                    {(new Date(game.date) < new Date()) && (
                        <Button
                            label='Add video'
                            onClick={addVideo}
                        />
                    )}
                </div>
            )}
        </>
    );
}