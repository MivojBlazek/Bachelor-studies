/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import GameDetail from '../../Components/referee/GameDetail';

export default function Game_detail() {
    return (
        <div style={styles.gameDetailContainer}>
            <GameDetail />
        </div>
    );
}

const styles = {
    gameDetailContainer: {
        padding: '0 3vw',
        display: 'flex',
        justifyContent: 'flex-start',
        alignItems: 'center',
        flexDirection: 'column', 
    },
};