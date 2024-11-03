import { Navigate, Outlet, useNavigate } from "react-router-dom";
import { useStateContext } from "../contexts/contextprovider";
import axiosClient from "../axiosClient";
import { useEffect, useState } from "react";
import Navigation from "../Components/delegate/Navigation";
import color from "../colors.jsx"

export default function DelegateLayout() {
    const { user, token, role, setUser, setToken, setRole } = useStateContext();
    const navigate = useNavigate();

    if (!token)
    {
        return <Navigate to="/login" />;
    }

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
            <main>
                <Outlet />
            </main>
        </div>
    );
}
