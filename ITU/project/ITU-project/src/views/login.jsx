import React from 'react';
import LoginForm from '../Components/LoginForm.jsx';

export default function Login() {
    return (
        <div style={{ padding: '20px', textAlign: 'center' }}>
            <h1>Login To Your Account</h1>
            <LoginForm />
        </div>
    );
}