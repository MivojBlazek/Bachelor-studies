import axios from "axios";
import { useEffect } from "react";
import { useContext } from "react";
import { Navigate, Outlet } from "react-router-dom";
import axiosClient from "../axiosClient";
import { useStateContext } from "../contexts/contextprovider";

export default function DefaultLayout() {
    const {user, token, role, setUser, setToken, setRole} = useStateContext();
    if (!token)
    {
        return <Navigate to='/login'/>
    }
    
    const onLogout = (ev) => {
        ev.preventDefault();
        axiosClient.get('/logout')
            .then(({}) => {
                setUser(null)
                setToken(null)
                setRole(null)
            })
    }

    useEffect(() => {
        axiosClient.get('/user')
            .then(({data}) => {
                setUser(data)
            })
    }, [])

    return (
        <div id="defaultLayout">
         <div className="content">
            <header>
                <div>
                    Header
                </div>
                <div>
                    {user.name}
                    <a href="#" onClick={onLogout}> Logout</a>
                </div>
            </header>
            <main>
                <Outlet />
            </main>
            </div>
        </div>
    )
}