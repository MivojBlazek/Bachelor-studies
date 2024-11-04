import React, { useEffect, useState } from 'react';
import { useParams } from 'react-router-dom';
import User from '../../Components/delegate/User';
import axiosClient from '../../axiosClient';

export default function RefereeProfile() {
    const { refereeId } = useParams();
    const [refereeUser, setRefereeUser] = useState(null);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchReferee = async () => {
            try
            {
                const response = await axiosClient.get(`/delegate/referee_profile/${refereeId}`);
                setRefereeUser(response.data);
            }
            catch (error)
            {
                setError(error.message);
            }
        };

        fetchReferee();
    }, [refereeId]);

    if (!refereeUser)
    {
        return null;
    }

    return (
        <div style={{ padding: '20px', textAlign: 'center' }}>
            <User user={refereeUser} />
        </div>
    );
}