/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React from 'react';
import { useLocation, useNavigate } from 'react-router-dom';
import { BrowserRouter as Router, Routes, Route, Link} from 'react-router-dom';

export default function Rating() {
    const location = useLocation();
    const navigate = useNavigate();

    const { game } = location.state;

    const photoUrl = game?.game?.delegate?.photo;

    const handleRatingClick = () => {
        navigate(`/referee/played_matches`);
    };
    
    return (
        <div style={styles.rating_container}>
            <div style={styles.delegate_back}>
                <button style={styles.backButton} onClick={(e) => {e.preventDefault(); handleRatingClick();}}>
                    Back
                </button></div>
            
            <div style={styles.delegate_info}>
                <img 
                    style={styles.delegate_photo} 
                    src={photoUrl} 
                    alt={"Profile picture"} 
                />
                <div style={styles.delegate_details}>
                    <p style={styles.nameTitle}>{game?.game?.delegate?.name}</p>
                    <p style={styles.addressContainer}>{game?.game?.delegate?.address}</p>
                    <p style={styles.contacts}>{game?.game?.delegate?.phone}</p>
                    <p style={styles.contacts}>{game?.game?.delegate?.email}</p>
                </div>
            </div>

            <div style={styles.feedback_section}>
                <h1 style={styles.mainTitle}>Rating</h1>
                <div style={styles.line}></div>
                <p>{game?.feedback}</p>
            </div>
        </div>
    );
}

const styles = {
    rating_container: {
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'flex-start',
        justifyContent: 'flex-start',
        fontFamily: 'Arial, sans-serif',
        width: '65vw'
    },

    nameTitle: {
        fontSize: 'clamp(10px, 3.2vh, 50px)',
        fontWeight: 'bold',
        marginBottom: '5px',
        marginTop: '0px',
    },

    contacts: {
        fontSize: 'clamp(10px, 2.2vh, 50px)',
        marginBottom: '0px',
    },

    addressContainer: {
        marginTop: '5px',
    },
    
    delegate_info: {
        display: 'flex',
        alignItems: 'flex-start',
        justifyContent: 'flex-start',
        marginBottom: '20px',
    },

    delegate_back: {
        paddingBottom:'30px',
    },
    
    delegate_photo: {
        width: '20vh',
        height: '20vh',
        objectFit: 'cover',
        borderRadius: '10px',
        marginRight: '20px',
    },
    
    delegate_details: {
        fontSize: 'clamp(10px, 2.2vh, 50px)',
        display: 'flex',
        flexDirection: 'column',
    },
    
    feedback_section: {
        marginTop: '20px',
        fontSize: 'clamp(10px, 2.2vh, 50px)',
        borderTop: '1px solid #ddd',
        padding: '10px',
        width: '100%',
        backgroundColor: '#bdbdbd',
        borderRadius: '8px',
    },
    backButton: {
        padding: '10px 28px',
        marginBottom: '30px',
        fontSize: 'clamp(9px, 2.5vh, 20px)',
        fontWeight: 'bold',
        backgroundColor: '#3bb4cc',
        color: '#fff',
        border: '2px solid #3190a3',
        borderRadius: '5px',
        cursor: 'pointer',
    },
    mainTitle: {
        textAlign: 'center',
        fontSize: 'clamp(10px, 3.2vh, 50px)',
        width: '100%',
        marginBottom: '5px',
    },
    line: {
        height: '2px',
        backgroundColor: '#ccc',
        marginBottom: '10px',
        width: '100%',
    },
};
