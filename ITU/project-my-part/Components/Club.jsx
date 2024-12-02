/**
 * ITU project
 * 
 * File: Club.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React from 'react';

export default function Club({ user }) {
    return (
        <div>
            <img
                src={user.photo ? user.photo : 'https://cdn.pixabay.com/photo/2023/02/18/11/00/icon-7797704_640.png'}
                alt={`${user.name}'s profile`}
                style={{
                    width: '200px',
                    height: '200px',
                    border: 'none',
                    borderRadius: '100px',
                    boxShadow: '2px 4px 5px rgba(0, 0, 0, 0.3)',
                }}
            />
            <h2 style={{ marginBottom: '50px' }}>
                {user.name}
            </h2>

            <div
                style={{
                    margin: '0 auto',
                    backgroundColor: 'white',
                    width: 'fit-content',
                    padding: '15px 30px',
                    borderRadius: '15px',
                    border: '1px solid lightgray',
                    fontWeight: 'bold',
                    display: 'grid',
                    gridTemplateColumns: 'auto auto',
                    gap: '13px',
                    textAlign: 'left',
                }}
            >
                <span style={{ textAlign: 'right' }}>Phone:</span><span>{user.phone}</span>
                <span style={{ textAlign: 'right' }}>Email:</span><span>{user.email}</span>
                <span style={{ textAlign: 'right' }}>City:</span><span>{user.city}</span>
                <span style={{ textAlign: 'right' }}>Address:</span><span>{user.address}</span>

                <div style={{
                    borderTop: '1px solid lightgray',
                    gridColumn: '1 / -1',
                    alignSelf: 'center',
                    justifyContent: 'center',
                    margin: '10px auto 0px auto',
                    maxWidth: '800px',
                    display: 'grid',
                    gridTemplateColumns: 'auto auto auto',
                    gridAutoFlow: 'row',
                    gap: '17px',
                    textAlign: 'left',
                }}>
                    <h3 style={{ marginBottom: '0px' }}>Players:</h3>
                    <span></span>
                    <span></span>

                    <span>Name</span>
                    <span>Dress number</span>
                    <span>Role</span>
                    
                    {user.players.map(player => (
                        <>
                            <span>{player.name}</span>
                            <span>{player.dress_number}</span>
                            <span>{player.role}</span>
                        </>
                    ))}
                </div>
            </div>
        </div>
    );
}
