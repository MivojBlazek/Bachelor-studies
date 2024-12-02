/**
 * ITU project
 * 
 * File: VideoCard.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React from 'react';
import Card from './Card.jsx';

export default function VideoCard({ video, onClick }) {
    const getThumbnailUrl = (url) => {
        const youtubeRegex = /^(?:https?:\/\/)?(?:www\.)?(?:youtube\.com\/watch\?v=([a-zA-Z0-9_]+)|youtu\.be\/([a-zA-Z\d_]+))(?:&.*)?$/;
        const match = url.match(youtubeRegex);
        if (match && match[1])
        {
            return `https://img.youtube.com/vi/${match[1]}/mqdefault.jpg`;
        }
        return null;
    };

    const thumbnailUrl = getThumbnailUrl(video.url);

    const cardContent = (
        <div
            style={{
                display: 'flex',
                flexDirection: 'row',
                margin: '0px 20px',
                maxWidth: '900px',
                gap: '25px',
            }}
        >
            <div
                style={{
                    flex: '2',
                }}
            >
                {(thumbnailUrl && (
                    <img
                        src={thumbnailUrl} 
                        alt='Video thumbnail'
                        style={{
                            width: '100%',
                            height: 'auto',
                            borderRadius: '10px',
                        }}
                    />
                )) || (video.url)}
            </div>
            <div
                style={{
                    flex: '3',
                    textAlign: 'left',
                }}
            >
                <h4 style={{ margin: '10px 0px' }}>Description:</h4>
                {video.description}
            </div>
        </div>
    );

    return (
        <div>
            <Card onClick={onClick} content={cardContent} />
        </div>
        
    );
}
