import React, { useEffect, useState } from 'react';
import { useParams } from 'react-router-dom';
import User from '../../Components/delegate/User';
import axiosClient from '../../axiosClient';
import { useStateContext } from '../../contexts/contextprovider';
import ErrorMessage from '../../Components/delegate/ErrorMessage';

export default function DelegateProfile() {
    const { delegateId } = useParams();
    const [delegateUser, setDelegateUser] = useState(null);
    const [error, setError] = useState(null);
    const { user } = useStateContext();

    useEffect(() => {
        const fetchDelegate = async () => {
            try
            {
                const response = await axiosClient.get(`/delegate/delegate_profile/${delegateId}`);
                setDelegateUser(response.data);

                setError(null);
            }
            catch (error)
            {
                if (error.response)
                {
                    setError(error.response.data.error);
                }
                else
                {
                    setError(error.message);
                }
            }
        };

        fetchDelegate();
    }, [delegateId]);

    if (!delegateUser && !error)
    {
        return null;
    }

    return (
        <div style={{ padding: '20px', textAlign: 'center' }}>
            <ErrorMessage message={error} />
            {delegateUser && <User user={delegateUser} isMe={user.id === Number(delegateId)} />}
        </div>
    );
}