/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import PlayedMatches from '../../Components/referee/PlayedMatches';

export default function Played_matches() {
    return (
        <div style={styles.playedMatchesContainer}>
            <PlayedMatches />
        </div>
    );
}

const styles = {
    playedMatchesContainer: {
        padding: '0 3vw',
        display: 'flex',
        justifyContent: 'flex-start',
        alignItems: 'center',
        flexDirection: 'column', 
    },
};