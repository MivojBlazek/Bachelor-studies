/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import RegisteredMatches from '../../Components/referee/RegisteredMatches';

export default function All_matches() {
    return (
        <div style={styles.registeredMatchesContainer}>
            <RegisteredMatches />
        </div>
    );
}

const styles = {
    registeredMatchesContainer: {
        padding: '0 3vw',
        display: 'flex',
        justifyContent: 'flex-start',
        alignItems: 'center',
        flexDirection: 'column', 
    },
};