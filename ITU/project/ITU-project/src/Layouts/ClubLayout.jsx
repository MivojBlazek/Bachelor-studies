/**
 * ITU project
 * 
 * Author: Matyáš Sapík <xsapik02>
 */
import { Navigate, Outlet, useNavigate, useLocation } from "react-router-dom";  // Import useNavigate for programmatic navigation
import { useStateContext } from "../contexts/contextprovider";       // Import context to manage global state
import axiosClient from "../axiosClient";                            // Import axios for API calls
import { useEffect } from "react";                                   // Import useEffect for side-effects

export default function ClubLayout() {
    const { user, token, setUser, setToken } = useStateContext();
    const navigate = useNavigate();
    const location = useLocation();

    if (!token) {
        return <Navigate to="/login" />;
    }

    const onLogout = (ev) => {
        ev.preventDefault();
        axiosClient.get('/logout')
            .then(() => {
                setUser(null);
                setToken(null);
                navigate('/login');
            });
    };

    useEffect(() => {
        axiosClient.get('/user').then(({ data }) => {
            setUser(data);
        });
    }, [setUser]);

    const goToProfile = () => navigate('/club/profile');
    const goToMatchSchedule = () => navigate('/club/matchSchedule');
    const goToBoard = () => navigate('/club/dashboard');
    const goToPlayers = () => navigate('/club/players');

    const getPageLabel = () => {
        switch (location.pathname) {
            case '/club/profile':
                return 'Profile';
            case '/club/matchSchedule':
                return 'Match Schedule';
            case '/club/dashboard':
                return 'Board';
            case '/club/players':
                return 'Players';
            default:
                return 'Board';
        }
    };

    // Stylování pomocí objektů
    const styles = {
        layout: {
            fontFamily: 'Arial, sans-serif',
            color: '#333',
        },
        header: {
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'space-between', // Distributes space between the left and right sections
            backgroundColor: '#7D2D2F',
            padding: '10px 20px',
        },
        button: {
            margin: '0 10px',
            padding: '10px 15px',
            border: 'none',
            borderRadius: '5px',
            backgroundColor: '#A9A9A9',
            color: 'black',
            cursor: 'pointer',
        },
        buttonSecondary: {
            backgroundColor: 'blue',
            color: 'white',
        },
        pageLabel: {
            fontSize: '1.5rem',
            fontWeight: 'bold',
            flex: 1, // Makes the label take up remaining space
            textAlign: 'center', // Centers the text horizontally
            color: 'white',
            marginRight: '250px',
        },
        btndiv: {
            display: 'flex',
            gap: '10px', // Adds spacing between buttons
        },
        main: {
            marginTop: '20px',
        },
    };

    return (
        <div id="clubLayout" style={styles.layout}>
            <header style={styles.header}>
                <div style={styles.btndiv}>
                <button style={styles.button} onClick={goToMatchSchedule}>Match Schedule</button>
                <button style={styles.button} onClick={goToBoard}>Board</button>
                <button style={styles.button} onClick={goToPlayers}>Players</button>
                </div>
                <div>
                    <h3 style={styles.pageLabel}>{getPageLabel()}</h3>
                </div>
                <div style={styles.btndiv}>
                <button style={styles.button} onClick={goToProfile}>Profile</button>
                <button style={{ ...styles.button, ...styles.buttonSecondary }} onClick={onLogout}>Logout</button>
                </div>
            </header>
            <main style={styles.main}>
                <Outlet />
            </main>
        </div>
    );
}
