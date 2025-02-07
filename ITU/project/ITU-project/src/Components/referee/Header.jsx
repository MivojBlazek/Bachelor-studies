/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import { Link } from 'react-router-dom';

const Header = ({ user, onLogout }) => {
    return (
        <header style={styles.header}>
            <div style={styles.leftSection}>
                <img src="https://rozhodci.ceskyhokej.cz/images/logo.png" alt="Logo" style={styles.logo} />
            </div>
            <div style={styles.centerSection}>
                <Link to="/referee/profile" style={styles.profileLink}>
                    <img 
                        src={user.photo} 
                        alt={"Profile picture"} 
                        style={styles.profileImage} 
                    />
                    <span>{user.name}</span>
                </Link>
            </div>
            <div style={styles.rightSection}>
                <button onClick={onLogout} style={styles.logoutButton}>
                    Logout
                </button>
            </div>
        </header>
    );
};

const styles = {
    header: {
        width: '100vw',
        height: '10vh',
        backgroundColor: '#0B0136',
        color: 'white',
        display: 'flex',
        justifyContent: 'space-between',
        alignItems: 'center',
        fontSize: 'clamp(10px, 4vh, 40px)',
        fontFamily: 'Arial, sans-serif', 
        position: 'fixed',
        top: 0,
        left: 0,
        zIndex: 1000,
    },
    leftSection: {
        width: '15vw',
        display: 'flex',
        justifyContent: 'flex-start',
    },
    centerSection: {
        flex: 2,
        display: 'flex',
        justifyContent: 'center',
        alignItems: 'center',
        gap: '10px',
    },
    rightSection: {
        width: '15vw',
        display: 'flex',
        justifyContent: 'flex-end',
        margin: '0 50px 0 0',
    },
    logo: {
        height: '7vh',
        width: '6vh',
        margin: '0 0 0 50px'
    },
    profileLink: {
        color: 'white',
        textDecoration: 'none',
        fontWeight: 'bold',
        display: 'flex',
        alignItems: 'center',
        gap: '10px',
    },
    profileImage: {
        height: 'clamp(10px, 7vh, 55px)',
        width: 'clamp(10px, 7vh, 55px)',
        borderRadius: '50%',
        objectFit: 'cover',
        fontSize: 'clamp(5px, 1vh, 10px)',
        fontWeight: 'normal',
    },
    logoutButton: {
        backgroundColor: '#ff4d4d',
        border: 'none',
        color: 'white',
        padding: 'clamp(2px, 2vh, 15px) clamp(5px, 4vw, 50px)',
        cursor: 'pointer',
        fontSize: 'clamp(5px, 5vh, 20px)',
        fontFamily: 'Arial, sans-serif', 
        borderRadius: '10px',
    }
};

export default Header;
