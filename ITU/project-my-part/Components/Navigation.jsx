import React from 'react';
import color from '../../colors.jsx'
import Button from './Button.jsx'

export default function Navigation({ currentPath, onNavigate, onLogout }) {
    const buttons = [
        { label: 'Dashboard', path: '/delegate/dashboard' },
        { label: 'Game list', path: '/delegate/game_list' },
        { label: 'Profile', path: '/delegate/profile' },
    ];

    const noBackButton = [
        '/delegate/dashboard',
        '/delegate/game_list',
        '/delegate/profile',
    ];

    return (
        <nav style={{
            display: 'flex',
            justifyContent: 'space-between',
            alignItems: 'center',
            backgroundColor: color.navigation,
            padding: '10px',
            marginBottom: '60px',
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
