/**
 * ITU project
 * 
 * File: referee_profile.jsx
 * 
 * Author: Michal Blažek <xblaze38>
 */

import React, { useEffect, useState } from 'react';
import { useParams } from 'react-router-dom';
import User from '../../Components/delegate/User';
import axiosClient from '../../axiosClient';
import ErrorMessage from '../../Components/delegate/ErrorMessage';

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

        fetchReferee();
    }, [refereeId]);

    if (!refereeUser && !error)
    {
        return null;
    }

    return (
        <div style={{ padding: '20px', textAlign: 'center' }}>
            <ErrorMessage message={error}/>
            {refereeUser && <User user={refereeUser} />}
        </div>
    );
}