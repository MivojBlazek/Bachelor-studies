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
            <p style={{ fontWeight: 'bold' }}>Phone: {user.phone}</p>
            <p style={{ fontWeight: 'bold' }}>Email: {user.email}</p>
            <p style={{ fontWeight: 'bold' }}>City: {user.city}</p>
            <p style={{ fontWeight: 'bold' }}>Address: {user.address}</p>
            <div style={{
                alignSelf: 'center',
                justifyContent: 'center',
                margin: '0px auto',
                maxWidth: '800px',
                display: 'grid',
                gridTemplateColumns: 'auto auto auto',
                gridAutoFlow: 'row',
                gap: '20px',
                textAlign: 'left',
            }}>
                <h3 style={{ marginBottom: '0px' }}>Players:</h3>
                <span></span>
                <span></span>

                <span style={{ fontWeight: 'bold' }}>Name</span>
                <span style={{ fontWeight: 'bold' }}>Dress number</span>
                <span style={{ fontWeight: 'bold' }}>Role</span>
                
                {user.players.map(player => (
                    <>
                        <span>{player.name}</span>
                        <span>{player.dress_number}</span>
                        <span>{player.role}</span>
                    </>
                ))}
            </div>
        </div>
    );
}
