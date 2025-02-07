/**
 * ITU project
 * 
 * Author: Matyáš Sapík <xsapik02>
 */
import { useState, useEffect } from "react";
import axiosClient from "../../axiosClient";

export default function Profile() {
    const [club, setClub] = useState(null);
    const [errors, setErrors] = useState(null);

    useEffect(() => {
      const fetchClub = async () => {
        try {
          const { data } = await axiosClient.get(`/club/profileshow`);
          setClub(data);
        } catch (err) {
          setErrors({ message: 'Error fetching data or club not found' });
        }
      };

      fetchClub();
    }, []);

    const handleChange = (e) => {
      const { name, value } = e.target;
      setClub((prevUser) => ({
        ...prevUser,
        [name]: value,
      }));
    };

    const onSubmit = async (ev) => {
        ev.preventDefault();
        try {
          await axiosClient.post('/club/profileupdate', club);
          window.alert("Saved successfully!");
        } catch (err) {
          setErrors(err);
          window.alert("An error occurred while saving.");
        }
      };

    const defaultImage = "https://cdn.pixabay.com/photo/2015/10/05/22/37/blank-profile-picture-973460_1280.png";

    const styles = {
      container: {
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        fontFamily: 'Arial, sans-serif',
      },
      form: {
        display: 'grid',
        gridTemplateColumns: '150px 1fr',
        gap: '10px 20px',
        maxWidth: '400px',
        marginTop: '20px',
      },
      label: {
        textAlign: 'right',
        paddingRight: '10px',
      },
      input: {
        padding: '10px',
        border: '1px solid #ccc',
        borderRadius: '5px',
      },
      button: {
        marginTop: '20px',
        padding: '10px 20px',
        backgroundColor: '#007BFF',
        color: '#fff',
        border: 'none',
        borderRadius: '10px',
        cursor: 'pointer',
        fontSize: '16px',
        transform: 'translateX(110px)',
      },
      buttonHover: {
        backgroundColor: '#0056b3',
      },
      profileImage: {
        width: '200px',
        height: '200px',
        marginBottom: '20px',
      },
    };

    return (
      <div style={styles.container}>
        <img
          src={club?.photo ? club.photo : defaultImage}
          alt="Profile"
          style={styles.profileImage}
        />
        <form onSubmit={onSubmit} style={styles.form}>
          <label htmlFor="clubName" style={styles.label}>Club name:</label>
          <input
            id="clubName"
            name="name"
            value={club && club.name}
            onChange={handleChange}
            style={styles.input}
          />
          <label htmlFor="city" style={styles.label}>City Name:</label>
          <input
            id="city"
            name="city"
            value={club && club.city}
            onChange={handleChange}
            style={styles.input}
          />
          <label htmlFor="address" style={styles.label}>Address:</label>
          <input
            id="address"
            name="address"
            value={club && club.address}
            onChange={handleChange}
            style={styles.input}
          />
          <label htmlFor="bank_account" style={styles.label}>Account Number:</label>
          <input
            id="bank_account"
            name="bank_account"
            value={club && club.bank_account}
            onChange={handleChange}
            style={styles.input}
          />
          <label htmlFor="phone" style={styles.label}>Phone Number:</label>
          <input
            id="phone"
            name="phone"
            value={club && club.phone}
            onChange={handleChange}
            style={styles.input}
          />
          <label htmlFor="email" style={styles.label}>Email:</label>
          <input
            id="email"
            name="email"
            value={club && club.email}
            onChange={handleChange}
            style={styles.input}
          />
          <label htmlFor="username" style={styles.label}>Username:</label>
          <input
            id="username"
            name="username"
            value={club && club.username}
            onChange={handleChange}
            readOnly
            style={styles.input}
          />
          <button
            type="submit"
            style={styles.button}
            onMouseOver={(e) => e.target.style.backgroundColor = styles.buttonHover.backgroundColor}
            onMouseOut={(e) => e.target.style.backgroundColor = styles.button.backgroundColor}
          >
            Save
          </button>
        </form>
      </div>
    );
  }
