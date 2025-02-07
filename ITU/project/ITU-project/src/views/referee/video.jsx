/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import Videos from '../../Components/referee/Videos';

export default function Video() {
    return (
        <div style={styles.videoContainer}>
            <Videos />
        </div>
    );
}

const styles = {
    videoContainer: {
        padding: '0 3vw',
        display: 'flex',
        justifyContent: 'flex-start',
        alignItems: 'center',
        flexDirection: 'column', 
    },
};