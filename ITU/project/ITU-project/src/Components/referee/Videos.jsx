/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React, { useState, useEffect } from 'react';
import { useLocation, useNavigate } from 'react-router-dom';

export default function Videos() {
    const location = useLocation();
    const navigate = useNavigate();
    const { videos } = location.state;

    console.log(videos);
    
    const handleBackClick = () => {
        navigate(-1);
    };
    return (
        <div style={styles.container}>
            <button onClick={handleBackClick} style={styles.backButton}>BACK</button>
            {videos && videos.length > 0 ? (
                videos.map((video, index) => (
                    <div key={index} style={styles.videoItem}>
                        <div style={styles.videoWrapper}>
                            <div style={styles.videoContainer}>
                                <video width="100%" controls style={styles.video}>
                                    <source src={video.url} type="video/mp4" />
                                    Your browser does not support the video tag.
                                </video>
                            </div>
                            <div style={styles.descriptionContainer}>
                                <h2>Description:</h2>
                                <p>{video.description}</p>
                            </div>
                        </div>
                    </div>
                ))
            ) : (
                <p>No videos available</p>
            )}
        </div>
    );
}

const styles = {
    container: {
        padding: '20px',
        textAlign: 'center',
        fontFamily: 'Arial, sans-serif',
    },
    videoItem: {
        marginBottom: '20px',
        border: '1px solid #ccc',
        borderRadius: '10px',
        backgroundColor: '#bdbdbd',
        overflow: 'hidden',
        padding: '10px',
    },
    videoWrapper: {
        display: 'flex',
        justifyContent: 'space-between',
        alignItems: 'stretch',
    },
    videoContainer: {
        marginRight: '20px',
        width:'50%',
    },
    video: {
        borderRadius: '10px',
    },
    descriptionContainer: {
        fontSize: 'clamp(10px, 2vh, 40px)',
        width: '45%',
        textAlign: 'left',
        backgroundColor: '#62d8f0',
        borderRadius: '10px',
        padding: '10px',
        border: '2px solid #3bb4cc',
    },
    backButton: {
        backgroundColor: '#4CAF50',
        color: 'white',
        padding: '10px 20px',
        border: 'none',
        borderRadius: '5px',
        cursor: 'pointer',
        marginBottom: '20px',
        fontSize: '16px',
    },
};
