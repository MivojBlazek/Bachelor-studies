/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import Rating from '../../Components/referee/Rating';

export default function RatingGame() {
    return (
        <div style={styles.ratingContainer}>
            <Rating />
        </div>
    );
}

const styles = {
    ratingContainer: {
        padding: '0 3vw',
        display: 'flex',
        justifyContent: 'flex-start',
        alignItems: 'center',
        flexDirection: 'column', 
    },
};