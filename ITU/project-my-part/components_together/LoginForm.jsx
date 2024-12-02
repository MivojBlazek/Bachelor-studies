/**
 * ITU project
 * 
 * File: LoginForm.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React, { useRef, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import TextInput from '../TextInput.jsx';
import Button from './Button.jsx';
import ErrorMessage from './ErrorMessage.jsx';
import axiosClient from '../axiosClient';
import { useStateContext } from '../contexts/contextprovider';

export default function LoginForm() {
    const navigate = useNavigate();
    const usernameRef = useRef();
    const passwordRef = useRef();
    const [error, setError] = useState(null);

    const { setUser, setToken, setRole } = useStateContext();

    const Submit = (ev) => {
        ev.preventDefault();
        const payload = {
            username: usernameRef.current.value,
            password: passwordRef.current.value,
        };

        axiosClient
            .post('/login', payload)
            .then(({ data }) => {
                setUser(data.user);
                setToken(data.token);
                setRole(data.role);
                localStorage.setItem('role', data.role);
                setError(null);

                if (data.role === 'delegate')
                {
                    //TODO uncomment this line after adding dashboard.jsx to router.jsx
                    navigate('/delegate/dashboard');
                }
                else if (data.role === 'club')
                {
                    //TODO uncomment this line after adding dashboard.jsx to router.jsx
                    // navigate('/club/dashboard');
                }
                else if (data.role === 'referee')
                {
                    //TODO uncomment this line after adding dashboard.jsx to router.jsx
                    // navigate('/referee/dashboard');
                }
                else
                {
                    navigate('/');
                }
            })
            .catch((err) => {
                const response = err.response;
                if (response && response.status === 422)
                {
                    setError("Invalid credentials!")
                }
            });
    };

    return (
        <form onSubmit={Submit} style={{ maxWidth: '300px', margin: '0 auto' }}>
            <TextInput reference={usernameRef} type="text" placeholder="Username" />
            <TextInput reference={passwordRef} type="password" placeholder="Password" />
            <Button label="Login" />
            <ErrorMessage message={error} />
        </form>
    );
}
