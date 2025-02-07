/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import { BrowserRouter as Router, Routes, Route, Link, useLocation } from 'react-router-dom';

const styles = {
    container: {
        display: 'flex',
        flexDirection: 'row',
        height: '80vh',
        marginTop: '5vh', 
    },
    sidebar: {
        width: '10vw',
        backgroundColor: '#5E6363',
        color: 'white',
        padding: '15px',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center', 
        justifyContent: 'flex-start',
        borderRadius: '0 15px 15px 0',
        fontFamily: 'Arial, sans-serif', 
        fontSize: 'clamp(10px, 1.5vw, 28px)',
        position: 'fixed',
        top: '15vh',
        left: 0,
        height: '70vh',
    },
    link: {
        color: 'white',
        textDecoration: 'none',
        display: 'block',
        padding: '10px 10px',
        textAlign: 'center',
        borderRadius: '10px'
    },
    activeLink: {
        backgroundColor: '#505050', 
    },
};

const Navigation = () => {
    const location = useLocation();

    const getLinkStyle = (path) => {
        return location.pathname.startsWith(path)
            ? { ...styles.link, ...styles.activeLink }
            : styles.link;
    };
    return (
        <div style={styles.container}>
        <nav style={styles.sidebar}>
            <ul style={{listStyle: 'none', padding:0}}>
                <li><Link style={getLinkStyle('/referee/dashboard')} to="/referee/dashboard">Dashboard</Link></li>
                <li><Link style={getLinkStyle('/referee/registered_matches')} to="/referee/registered_matches">Registered matches</Link></li>
                <li><Link style={getLinkStyle('/referee/all_matches')} to="/referee/all_matches">All matches</Link></li>
                <li><Link style={getLinkStyle('/referee/played_matches')} to="/referee/played_matches">Played matches</Link></li>
                <li><Link style={getLinkStyle('/referee/payments')} to="/referee/payments">Payments</Link></li>
                <li><Link style={getLinkStyle('/referee/profile')} to="/referee/profile">Profile</Link></li>
            </ul>
        </nav>
        </div>
    );
};

export default Navigation;