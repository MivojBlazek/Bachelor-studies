import React, { useState, useEffect } from 'react';
import { FaBars, FaLongArrowAltLeft, FaSignOutAlt, FaHome, FaUser, FaMoneyBillWave} from 'react-icons/fa';
import { MdSportsHockey } from "react-icons/md";
import color from '../../colors.jsx'
import Button from './Button.jsx'

export default function Navigation({ currentPath, onNavigate, onLogout }) {
    const [mobileView, setMobileView] = useState(false);
    const [mobileMenuActive, setMobileMenuActive] = useState(false);

    const buttons = [
        { label: 'Dashboard', path: '/delegate/dashboard', icon: <FaHome style={{ fontSize: '17px' }} /> },
        { label: 'Games', path: '/delegate/game_list', icon: <MdSportsHockey style={{ fontSize: '17px' }} /> },
        { label: 'Profile', path: '/delegate/profile', icon: <FaUser style={{ fontSize: '17px' }} /> },
        { label: 'Payments', path: '/delegate/allPayments', icon: <FaMoneyBillWave style={{ fontSize: '17px' }} /> },
    ];

    const noBackButton = [
        '/delegate/dashboard',
        '/delegate/game_list',
        '/delegate/profile',
        '/delegate/allPayments',
    ];

    const handleResize = () => {
        if (window.innerWidth <= 768) {
            setMobileView(true);
        } else {
            setMobileView(false);
        }
    };

    useEffect(() => {
        handleResize();
        window.addEventListener('resize', handleResize);
        return () => window.removeEventListener('resize', handleResize);
    }, []);

    const toggleMobileMenu = () => {
        setMobileMenuActive((prev) => !prev);
    };

    return (
        <nav
            style={{
                display: 'flex',
                flexDirection: 'row',
                flexWrap: 'nowrap',
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
            }}
        >

            {mobileView && (
                <button
                    onClick={toggleMobileMenu}
                    style={{
                        background: 'none',
                        border: 'none',
                        color: color.secondary,
                        fontSize: '30px',
                        marginLeft: '10px',
                    }}
                >
                    <FaBars />
                </button>
            )}
            <div
                style={{
                    display: mobileView && !mobileMenuActive ? 'none' : 'flex',
                    flexDirection: mobileView ? 'column' : 'row',
                    position: mobileView ? 'absolute' : '',
                    backgroundColor: mobileView ? color.navigation : '',
                    top: mobileView ? '60px' : '',
                    left: mobileView ? '0px' : '',
                    padding: mobileView ? '0px 12px 12px 12px' : '',
                    borderRadius: mobileView ? '0px 0px 25px 0px' : '',
                }}
            >
                {buttons.map(({ label, path, icon }) => (
                    <Button
                        key={path}
                        label={<span style={{ display: 'flex', alignItems: 'center' }}>{icon}&nbsp;&nbsp;{label}</span>}
                        onClick={() => { onNavigate(path); setMobileMenuActive(false); }}
                        isActive={currentPath === path}
                    />
                ))}
            </div>
            <div
                style={{
                    display: 'flex',
                    justifyContent: 'flex-end',
                    width: mobileView ? '100%' : 'auto',
                }}
            >
                {!noBackButton.includes(currentPath) && (
                    (!mobileView) ? (
                        <Button
                            label='Back'
                            onClick={() => window.history.back()}
                        />
                    ) : (
                        <button
                            onClick={() => window.history.back()}
                            style={{
                                background: 'none',
                                border: 'none',
                                color: color.secondary,
                                fontSize: '30px',
                                padding: '5px',
                                margin: '5px 20px',
                            }}
                        >
                            <FaLongArrowAltLeft />
                        </button>
                    )
                )}
                {(!mobileView) ? (
                    <Button
                        label='Logout'
                        onClick={onLogout}
                    />
                ) : (
                    <button
                        onClick={onLogout}
                        style={{
                            background: 'none',
                            border: 'none',
                            color: '#FF0000',
                            fontSize: '30px',
                            padding: '5px',
                            margin: '5px',
                        }}
                    >
                        <FaSignOutAlt />
                    </button>
                )}
            </div>
        </nav>
    );
}
