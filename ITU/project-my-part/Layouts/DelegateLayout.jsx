/**
 * ITU project
 * 
 * File: DelegateLayout.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import { Navigate, Outlet, useNavigate } from "react-router-dom";
import { useStateContext } from "../contexts/contextprovider";
import axiosClient from "../axiosClient";
import { useEffect, useState } from "react";
import Navigation from "../Components/delegate/Navigation";
import color from "../colors.jsx"

export default function DelegateLayout() {
    const { user, token, role, setUser, setToken, setRole } = useStateContext();
    const navigate = useNavigate();

    /** If user is not sign in */
    if (!token)
    {
        return <Navigate to="/login" />;
    }

    /** Logout user */
    const onLogout = (ev) => {
        axiosClient.get('/logout')
            .then(() => {
                setUser(null);
                setToken(null);
                setRole(null);
                localStorage.removeItem('role');
                navigate('/login');
            });
    };

    useEffect(() => {
        axiosClient.get('/user').then(({ data }) => {
            setUser(data);
        });

        // Setting default document style
        return () => {
            document.body.style.margin = '0';
            document.body.style.backgroundColor = color.background;
        };
    }, []);


    const navigation = (path) => {
        navigate(path);
    };

    return (
        <div id="delegateLayout">
            <header>
                <Navigation
                    currentPath={window.location.pathname}
                    onNavigate={navigation}
                    onLogout={onLogout}
                />
            </header>
            <main style={{ paddingTop: '100px' }}>
                {/* Rest of the website based on current view */}
                <Outlet />
            </main>
        </div>
    );
}
