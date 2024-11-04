import React, { useEffect, useState } from 'react';
import { useParams } from 'react-router-dom';
import User from '../../Components/delegate/User';
import axiosClient from '../../axiosClient';

export default function DelegateProfile() {
    const { delegateId } = useParams();
    const [delegateUser, setDelegateUser] = useState(null);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchDelegate = async () => {
            try
            {
                const response = await axiosClient.get(`/delegate/delegate_profile/${delegateId}`);
                setDelegateUser(response.data);
            }
            catch (error)
            {
                setError(error.message);
            }
        };

        fetchDelegate();
    }, [delegateId]);

    if (!delegateUser)
    {
        return null;
    }

    return (
        <div style={{ padding: '20px', textAlign: 'center' }}>
            <User user={delegateUser} />
        </div>
    );
}