import React, { useEffect, useState } from 'react';
import { useParams } from 'react-router-dom';
import Club from '../../Components/delegate/Club';
import axiosClient from '../../axiosClient';
import ErrorMessage from '../../Components/delegate/ErrorMessage';

export default function ClubProfile() {
    const { clubId } = useParams();
    const [clubUser, setClubUser] = useState(null);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchClub = async () => {
            try
            {
                const response = await axiosClient.get(`/delegate/club_profile/${clubId}`);
                setClubUser(response.data);

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

        fetchClub();
    }, [clubId]);

    if (!clubUser && !error)
    {
        return null;
    }

    return (
        <div style={{ padding: '20px', textAlign: 'center' }}>
            <ErrorMessage message={error} />
            {clubUser && <Club user={clubUser} />}
        </div>
    );
}