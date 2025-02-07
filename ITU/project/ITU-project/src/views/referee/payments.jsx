/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import Payment from '../../Components/referee/Payment';

export default function payment() {
    return (
        <div style={styles.paymentContainer}>
            <Payment />
        </div>
    );
}

const styles = {
    paymentContainer: {
        padding: '0 3vw',
        display: 'flex',
        justifyContent: 'flex-start',
        alignItems: 'center',
        flexDirection: 'column', 
    },
};