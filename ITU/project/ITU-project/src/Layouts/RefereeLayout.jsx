/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import { Navigate, Outlet, Link } from "react-router-dom";
import { useStateContext } from "../contexts/contextprovider";
import axiosClient from "../axiosClient";
import { useEffect } from "react";
import Navigation from "../Components/referee/Navigation";
import Header from "../Components/referee/Header";

export default function RefereeLayout() {
    const { user, token, setUser, setToken } = useStateContext();

    if (!token)
    {
        return <Navigate to="/login" />;
    }

    const onLogout = (ev) => {
        ev.preventDefault();
        axiosClient.get('/logout')
            .then(() => {
                setUser(null);
                setToken(null);
            });
    };

    useEffect(() => {
        axiosClient.get('/user').then(({ data }) => {
            setUser(data);
        });
    }, []);

    return (
        <div id="refereeLayout" style={styles.layout}>
            <Header user={user} onLogout={onLogout}/>
            <div style={styles.container}>
                <Navigation /> 

                <main style={styles.main}> 
                    <Outlet />
                </main>
            </div>
        </div>
    );
}

const styles = {
    layout: {
        display: 'flex',
        flexDirection: 'column',
        height: '100vh',
    },
    container: {
        display: 'flex',
        flexDirection: 'row',
        marginTop: '15vh', 
        height: '85vh',
    },
    main: {
        flex: 1,
        padding: '20px',
        marginLeft: '15vw',
        //overflowY: 'auto',
    }
};