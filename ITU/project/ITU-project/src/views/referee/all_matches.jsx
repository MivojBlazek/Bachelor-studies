/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import AllMatches from '../../Components/referee/AllMatches';

export default function All_matches() {
    return (
        <div style={styles.AllMatchesContainer}>
            <AllMatches />
        </div>
    );
}

const styles = {
    AllMatchesContainer: {
        padding: '0 3vw',
        display: 'flex',
        justifyContent: 'flex-start',
        alignItems: 'center',
        flexDirection: 'column', 
    },
};