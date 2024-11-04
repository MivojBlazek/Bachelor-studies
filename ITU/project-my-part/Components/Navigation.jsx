import React from 'react';
import color from '../../colors.jsx'
import Button from './Button.jsx'

export default function Navigation({ currentPath, onNavigate, onLogout }) {
    const buttons = [
        { label: 'Dashboard', path: '/delegate/dashboard' },
        { label: 'Games', path: '/delegate/game_list' },
        { label: 'Profile', path: '/delegate/profile' },
        { label: 'Payments', path: '/delegate/allPayments' },
    ];

    const noBackButton = [
        '/delegate/dashboard',
        '/delegate/game_list',
        '/delegate/profile',
        '/delegate/allPayments',
    ];

    return (
        <nav style={{
            display: 'flex',
            justifyContent: 'space-between',
            alignItems: 'center',
            backgroundColor: color.navigation,
            padding: '10px',
            marginBottom: '60px',
            position: 'fixed',
            top: '0px',
            left: '0px',
            right: '0px',
            zIndex: '10',
        }}>
            <div style={{ display: 'flex' }}>
                {buttons.map(({ label, path }) => (
                    <Button
                        key={path}
                        label={label}
                        onClick={() => onNavigate(path)}
                        isActive={currentPath === path}
                    />
                ))}
            </div>
            <div style={{ display: 'flex' }}>
                {!noBackButton.includes(currentPath) && (
                    <Button
                        label='Back'
                        onClick={() => window.history.back()}
                    />
                )}
                <Button
                    label='Logout'
                    onClick={onLogout}
                />
            </div>
        </nav>
    );
}
