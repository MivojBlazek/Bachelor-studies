/**
 * ITU project
 * 
 * File: profile.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React from 'react';
import User from '../../Components/delegate/User';
import { useStateContext } from '../../contexts/contextprovider';

export default function Profile() {
    const { user } = useStateContext();
    
    return (
        <div style={{ padding: '20px', textAlign: 'center' }}>
            <User user={user} isMe={true} />
        </div>
    );
}