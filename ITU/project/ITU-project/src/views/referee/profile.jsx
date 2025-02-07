/**
 * ITU project
 * 
 * Author: Matěj Lepeška <xlepes00>
 */

import React, { useEffect, useState } from 'react';
import axiosClient from '../../axiosClient';
import RefInput from '../../Components/referee/ref_input'; 

export default function Profile() {
    const [referee, setReferee] = useState(null);
    const [name, setName] = useState('');
    const [birthDate, setBirthDate] = useState('');
    const [address, setAddress] = useState('');
    const [phone, setPhone] = useState('');
    const [email, setEmail] = useState('');
    const [bankAccount, setBankAccount] = useState('');
    const [license, setLicense] = useState('');
    useEffect(() => {
        const fetchReferee = async () => {
            try {
                const response = await axiosClient.get('/referee/profile');
                const data = response.data;
                setReferee(data);
                setName(data.name);
                setBirthDate(data.birth_date);
                setAddress(data.address);
                setPhone(data.phone);
                setEmail(data.email);
                setBankAccount(data.bank_account);
                setLicense(data.license);
            } catch (error) {
                console.error('Chyba při načítání profilu uživatele', error);
            }
        };

        fetchReferee();
    }, []);

    const handleBlur = async (updatedData) => {
        try {
            const response = await axiosClient.put('/referee/profile', updatedData);
            console.log('Data byla úspěšně uložena:', response.data);
        } catch (error) {
            console.error('Chyba při ukládání změn:', error);
        }
    };

    if (!referee) {
        return <div>Loading...</div>;
    }
    return (
        <div style={styles.container}>
            <h1 style={styles.heading}>Profile</h1>
            <div style={styles.profileImageContainer}><img src={referee.photo} alt="Profile photo" style={styles.profileImage}/></div>
            <div style={styles.inputContainer}>
                <RefInput 
                    placeholder="Name" 
                    value={name}
                    onChange={(e) => setName(e.target.value)} 
                    onBlur={() => handleBlur({ name })} />
                <RefInput 
                    placeholder="Date of birth" 
                    value={birthDate}
                    onChange={(e) => setBirthDate(e.target.value)} 
                    onBlur={() => handleBlur({ birth_date: birthDate })} />
                <RefInput 
                    placeholder="Address" 
                    value={address}
                    onChange={(e) => setAddress(e.target.value)} 
                    onBlur={() => handleBlur({ address })} />
                <RefInput 
                    placeholder="Phone" 
                    value={phone}
                    onChange={(e) => setPhone(e.target.value)} 
                    onBlur={() => handleBlur({ phone })} />
                <RefInput 
                    placeholder="Email" 
                    value={email}
                    onChange={(e) => setEmail(e.target.value)} 
                    onBlur={() => handleBlur({ email })} />
                <RefInput 
                    placeholder="Bank account" 
                    value={bankAccount}
                    onChange={(e) => setBankAccount(e.target.value)} 
                    onBlur={() => handleBlur({ bank_account: bankAccount })} />
                <RefInput 
                    placeholder="License" 
                    value={license}
                    onChange={(e) => setLicense(e.target.value)} 
                    onBlur={() => handleBlur({ license })} />
            </div>
        </div>
    );
}

const styles = {
    container: {
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        fontFamily: 'Arial, sans-serif', 
        fontSize: 'clamp(10px, 1.5vw, 40px)',
    },
    heading: {
        textAlign: 'center',
        marginBottom: '20px',
    },
    profileImageContainer: {
        marginBottom: '20px',
    },
    profileImage: {
        borderRadius: '50%',
        border: '5px solid #007BFF',
        width: 'clamp(10px, 30vh, 600px)',
        height: 'clamp(10px, 30vh, 600px)',
        objectFit: 'cover',
    },
    inputContainer: {
        width: '50vw',
        display: 'flex',
        flexDirection: 'column',
        gap: '10px',
    }
};