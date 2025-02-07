/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React, { useEffect, useState } from 'react';
import Dashboar from '../../Components/referee/Dashboard';

export default function Dashboard() {
    return (
        <div style={styles.dashboardContainer}>
            <Dashboar />
        </div>
    );
}

const styles = {
    dashboardContainer: {
        padding: '0 3vw',
        display: 'flex',
        justifyContent: 'flex-start',
        alignItems: 'flex-end',
        flexDirection: 'column', 
    },
};