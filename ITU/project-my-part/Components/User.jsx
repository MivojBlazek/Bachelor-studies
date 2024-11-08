import React from 'react';

export default function User({ user, isMe = false }) {
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
            <p style={{ fontWeight: 'bold' }}>License: {user.license}</p>
            <p style={{ fontWeight: 'bold' }}>Date of birth: {user.birth_date}</p>
            <p style={{ fontWeight: 'bold' }}>Phone: {user.phone}</p>
            <p style={{ fontWeight: 'bold' }}>Email: {user.email}</p>
            <p style={{ fontWeight: 'bold' }}>Address: {user.address}</p>
            {isMe && (
                <p style={{ fontWeight: 'bold' }}>Bank account: {user.bank_account}</p>
            )}
        </div>
    );
}
