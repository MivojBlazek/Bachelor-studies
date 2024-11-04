import React, { useEffect, useState } from 'react';
import { useParams } from 'react-router-dom';
import axiosClient from '../../axiosClient';
import VideoCard from '../../Components/delegate/VideoCard';
import Button from '../../Components/delegate/Button';
import Modal from 'react-modal';

export default function GameVideos() {
    const { id } = useParams();
    const [game, setGame] = useState(null);
    const [videos, setVideos] = useState([]);
    const [modalIsOpen, setModalIsOpen] = useState(false);
    const [selectedVideo, setSelectedVideo] = useState(null);
    
    useEffect(() => {
        const fetchGame = async () => {
            try
            {
                const response = await axiosClient.get(`/delegate/games/${id}`);
                setGame(response.data);
                setVideos(response.data.videos);
            }
            catch (error)
            {
                setError(error.message);
            }
        };

        fetchGame();
    }, [id]);

    const openModal = (video) => {
        setModalIsOpen(true);
        setSelectedVideo(video);
    };

    const closeModal = () => {
        setModalIsOpen(false);
        setSelectedVideo(null);
        window.history.back();
    };

    const isValidUrl = (url) => {
        const youtubeRegex = /^(?:https?:\/\/)?(?:www\.)?(?:youtube\.com\/watch\?v=([a-zA-Z0-9_]+)|youtu\.be\/([a-zA-Z\d_]+))(?:&.*)?$/;
        return youtubeRegex.test(url);
    };

    if (!game)
    {
        return null;
    }

    return (
        <div style={{margin: '0px 20px', textAlign: 'center'}}>
            <h1>Videos:</h1>
            {videos.map(video => (
                <VideoCard 
                    key={video.id}
                    video={video}
                    onClick={() => openModal(video)} 
                />
            ))}

            <Modal
                isOpen={modalIsOpen}
                onRequestClose={closeModal}
                style={{
                    content: {
                        top: '50%',
                        left: '50%',
                        right: 'auto',
                        bottom: 'auto',
                        transform: 'translate(-50%, -50%)',
                        width: '80%',
                        maxWidth: '800px',
                        height: '80%',
                        maxHeight: '550px',
                    },
                }}
                ariaHideApp={false}
            >
                <div style={{ position: 'relative', height: '100%' }}>
                    <Button
                        label='Close'
                        onClick={closeModal}
                        style={{
                            marginBottom: '20px',
                            float: 'right',
                        }}
                    />
                    {selectedVideo && isValidUrl(selectedVideo.url) ? (
                        <iframe
                            style={{
                                width: '100%',
                                height: '87%',
                                border: 'none',
                            }}
                            src={selectedVideo.url.replace("watch?v=", "embed/")}
                            allowFullScreen
                        />
                    ) : (
                        <div style={{ textAlign: 'center', padding: '20%' }}>
                            <p style={{ fontSize: '25px' }}>Video not available.</p>
                        </div>
                    )}
                </div>
            </Modal>
        </div>
    );
}