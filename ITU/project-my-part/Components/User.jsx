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
                <span style={{ textAlign: 'right' }}>License:</span><span>{user.license}</span>
                <span style={{ textAlign: 'right' }}>Date of birth:</span><span>{(new Date(user.birth_date)).toLocaleDateString()}</span>
                <span style={{ textAlign: 'right' }}>Phone:</span><span>{user.phone}</span>
                <span style={{ textAlign: 'right' }}>Email:</span><span>{user.email}</span>
                <span style={{ textAlign: 'right' }}>Address:</span><span>{user.address}</span>
                {isMe && (
                    <>
                        <span style={{ textAlign: 'right' }}>Bank account:</span><span>{user.bank_account}</span>
                    </>
                )}
            </div>
        </div>
    );
}
