/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import PaymentEdit from '../../Components/referee/PaymentEdit';

export default function Payment_edit() {
    return (
        <div style={styles.paymentContainer}>
            <PaymentEdit />
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