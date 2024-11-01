import { Navigate, Outlet } from "react-router-dom";
import { useStateContext } from "../contexts/contextprovider";
import axiosClient from "../axiosClient";
import { useEffect } from "react";

export default function DelegateLayout() {
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
        <div id="delegateLayout">
            <header>
                <div>Delegate Header</div>
                <div>
                    {user.name} <a href="#" onClick={onLogout}>Logout</a>
                </div>
            </header>
            <main>
                <Outlet />
            </main>
        </div>
    );
}
