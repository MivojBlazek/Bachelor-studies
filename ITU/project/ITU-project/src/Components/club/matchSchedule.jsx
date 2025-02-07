/**
 * ITU project
 * 
 * Author: Matyáš Sapík <xsapik02>
 */
import React, { useState, useEffect } from "react";
import axiosClient from "../../axiosClient";

const MatchSchedule = () => {
  const [clubId, setClub] = useState(null); // ID přihlášeného klubu
  const [matches, setMatches] = useState([]);
  const [teams, setTeams] = useState([]);
  const [isLoading, setIsLoading] = useState(false);
  const [errors, setErrors] = useState(null);

  // Modální okna
  const [isEditModalOpen, setIsEditModalOpen] = useState(false);
  const [isAddModalOpen, setIsAddModalOpen] = useState(false);

  // Data pro editaci a přidání zápasu
  const [editMatch, setEditMatch] = useState(null);
  const [formData, setFormData] = useState({
    date: "",
    time: "",
    location: "",
    league: "",
    clubHome_id: "",
    clubVisitor_id: "",
  });

  useEffect(() => {
    if (clubId && clubId.id) {
      fetchMatches();
      fetchClubs();
    }
  }, [clubId]);

  // Načítání zápasů
  const fetchMatches = async () => {
    setIsLoading(true);
    try {
      const response = await axiosClient.get(`/club/match/${clubId}`);
      let sortedMatches = response.data.data || [];
      sortedMatches = sortedMatches.sort((a, b) => {
        const dateA = new Date(`${a.date}T${a.time}`);
        const dateB = new Date(`${b.date}T${b.time}`);
        return dateA - dateB;
      });
      setMatches(sortedMatches);
    } catch (err) {
      setErrors({ message: "Error fetching matches for this team." });
    } finally {
      setIsLoading(false);
    }
  };

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

  // Načítání týmů
  const fetchClubs = async () => {
    try {
      const response = await axiosClient.get("/club/clubs");
      setTeams(response.data.data);
    } catch (err) {
      console.error("Error fetching clubs:", err);
    }
  };

  // Funkce pro otevření Edit Popupu
  const handleEdit = (match) => {
    setEditMatch(match);
    setFormData({
      date: match.date,
      time: match.time,
      location: match.location,
      league: match.league,
      clubHome_id: match.clubHome_id,
      clubVisitor_id: match.clubVisitor_id,
    });
    setIsEditModalOpen(true);
  };

  // Uložení změn při editaci
  const handleEditSubmit = async () => {
    try {
      await axiosClient.put(`/club/match/${editMatch.id}`, formData);
      fetchMatches();
      setIsEditModalOpen(false);
    } catch (err) {
      console.error("Error updating match:", err);
      alert("Error updating match.");
    }
  };

  // Funkce pro otevření Add Popupu
  const handleAdd = () => {
    if (!clubId || !clubId.id) return;
    setFormData({
      date: "",
      time: "",
      location: "",
      league: "",
      clubHome_id: clubId.id,
      clubVisitor_id: "",
    });
    setIsAddModalOpen(true);
  };

  const handleAddSubmit = async () => {
    try {
      const currentDate = new Date();
      const selectedDate = new Date(formData.date);

      if (selectedDate < currentDate.setHours(0, 0, 0, 0)) {
        alert("The match date must be today or a future date.");
        return;
      }

      // Převedeme čas na formát H:i:s
      const formattedTime = new Date(`1970-01-01T${formData.time}`)
        .toTimeString()
        .split(" ")[0];

      const updatedData = {
        ...formData,
        time: formattedTime,
      };

      await axiosClient.post("/club/matchescreate", updatedData);
      fetchMatches();
      setIsAddModalOpen(false);
    } catch (err) {
      console.error("Error adding match:", err);
      alert("Error adding match. Please check the input fields.");
    }
  };

  const handleInputChange = (event) => {
    const { name, value } = event.target;
    setFormData((prevData) => ({
      ...prevData,
      [name]: value,
    }));
  };

  const handleDelete = async () => {
    try {
      // Pošleme požadavek na backend k odstranění zápasu
      await axiosClient.delete(`/club/match/${editMatch.id}`);
      fetchMatches(); // Aktualizuje seznam zápasů
      setIsEditModalOpen(false); // Zavře modal
    } catch (err) {
      console.error("Error deleting match:", err);
      alert("Error deleting match. Please try again.");
    }
  };

  return (
    <div>
      {clubId && (
        <button onClick={handleAdd} style={styles.addButton}>
            Add match
        </button>
        )}

      {isLoading ? (
        <p>Loading matches...</p>
      ) : errors ? (
        <p>{errors.message}</p>
      ) : (
        <table style={styles.table}>
          <thead>
            <tr>
              <th style={styles.tableHeader}>Date</th>
              <th style={styles.tableHeader}>Time</th>
              <th style={styles.tableHeader}>Stadium</th>
              <th style={styles.tableHeader}>League</th>
              <th style={styles.tableHeader}>Home</th>
              <th style={styles.tableHeader}>Visitor</th>
              <th style={styles.tableHeader}>Main</th>
              <th style={styles.tableHeader}>Line</th>
              <th style={styles.tableHeader}>Line</th>
              <th style={styles.tableHeader}>Delegate</th>
              <th style={styles.tableHeader}>Edit button</th>
            </tr>
          </thead>
          <tbody>
            {matches.length > 0 ? (
                    matches.map((match) => {
                const now = new Date();
                const matchDateTime = new Date(`${match.date}T${match.time}`);
                const timeDifference = matchDateTime - now;

                // Inicializace rozhodčích podle role
                let mainReferee = "";
                let lineReferee1 = "";
                let lineReferee2 = "";

                // Projdeme rozhodčí v controls a přiřadíme je do správných proměnných
                match.controls?.forEach((control) => {
                    if (control.refereeRole === "main") {
                    mainReferee = control.referee?.name || "";
                    } else if (control.refereeRole === "line") {
                    if (lineReferee1 === "") {
                        lineReferee1 = control.referee?.name || "";
                    } else {
                        lineReferee2 = control.referee?.name || "";
                    }
                    }
                });

                return (
                    <tr key={match.id}>
                    <td style={styles.tableCell}>{match.date}</td>
                    <td style={styles.tableCell}>{match.time}</td>
                    <td style={styles.tableCell}>{match.location}</td>
                    <td style={styles.tableCell}>{match.league}</td>
                    <td style={styles.tableCell}>{match.club1 ? match.club1.name : ""}</td>
                    <td style={styles.tableCell}>{match.club2 ? match.club2.name : ""}</td>
                    <td style={styles.tableCell}>{mainReferee}</td> {/* Hlavní rozhodčí */}
                    <td style={styles.tableCell}>{lineReferee1}</td> {/* První čárový rozhodčí */}
                    <td style={styles.tableCell}>{lineReferee2}</td> {/* Druhý čárový rozhodčí */}
                    <td style={styles.tableCell}>{match.delegate ? match.delegate.name : ""}</td>
                    <td style={styles.tableCell}>
                        {match.clubHome_id === clubId.id &&
                        timeDifference > 24 * 60 * 60 * 1000 && (
                            <button onClick={() => handleEdit(match)} style={styles.editButton}>Edit</button>
                        )}
                    </td>
                    </tr>
                );
                })
            ) : (
                <tr>
                <td colSpan="11">No matches found</td>
                </tr>
            )}
            </tbody>



        </table>
      )}

    {isEditModalOpen && (
    <div style={styles.modalOverlay}>
        <div style={styles.modalContent}>
        <h2 style={styles.modalTitle}>Edit Match</h2>

        <div style={styles.inputGroup}>
            <label style={styles.label}>Date:</label>
            <input
            type="date"
            name="date"
            value={formData.date}
            onChange={handleInputChange}
            style={styles.input}
            />
        </div>

        <div style={styles.inputGroup}>
            <label style={styles.label}>Time:</label>
            <input
            type="time"
            name="time"
            value={formData.time}
            onChange={handleInputChange}
            style={styles.input}
            />
        </div>

        <div style={styles.inputGroup}>
            <label style={styles.label}>Location:</label>
            <input
            type="text"
            name="location"
            value={formData.location}
            onChange={handleInputChange}
            style={styles.input}
            />
        </div>

        <div style={styles.inputGroup}>
            <label style={styles.label}>League:</label>
            <input
            type="text"
            name="league"
            value={formData.league}
            onChange={handleInputChange}
            style={styles.input}
            />
        </div>

        <div style={styles.inputGroup}>
            <label style={styles.label}>Home Club:</label>
            <span style={styles.spanText}>
            {teams.find((club) => club.id === formData.clubHome_id)?.name || "N/A"}
            </span>
        </div>

        <div style={styles.inputGroup}>
            <label style={styles.label}>Visitor Club:</label>
            <select
            name="clubVisitor_id"
            value={formData.clubVisitor_id}
            onChange={handleInputChange}
            style={styles.input}
            >
            {teams
                .filter((club) => club.id !== formData.clubHome_id)
                .map((club) => (
                <option key={club.id} value={club.id}>
                    {club.name}
                </option>
                ))}
            </select>
        </div>

        <div style={styles.modalActions}>
            <button onClick={handleEditSubmit} style={styles.primaryButton}>
            Save
            </button>
            <button
            onClick={() => setIsEditModalOpen(false)}
            style={styles.cancelButton}
            >
            Cancel
            </button>
            <button
            onClick={() => {
                if (window.confirm("Are you sure you want to delete this match?")) {
                handleDelete();
                }
            }}
            style={styles.deleteButton}
            >
            Delete
            </button>
        </div>
        </div>
    </div>
    )}


      {/* Add Match Modal */}
      {isAddModalOpen && (
  <div style={styles.modalOverlay}>
    <div style={styles.modalContent}>
      <h2 style={styles.modalTitle}>Add Match</h2>

      <div style={styles.inputGroup}>
        <label style={styles.label}>Date:</label>
        <input
          type="date"
          name="date"
          value={formData.date}
          onChange={handleInputChange}
          style={styles.input}
        />
      </div>

      <div style={styles.inputGroup}>
        <label style={styles.label}>Time:</label>
        <input
          type="time"
          name="time"
          value={formData.time}
          onChange={handleInputChange}
          style={styles.input}
        />
      </div>

      <div style={styles.inputGroup}>
        <label style={styles.label}>Location:</label>
        <input
          type="text"
          name="location"
          value={formData.location}
          onChange={handleInputChange}
          style={styles.input}
        />
      </div>

      <div style={styles.inputGroup}>
        <label style={styles.label}>League:</label>
        <input
          type="text"
          name="league"
          value={formData.league}
          onChange={handleInputChange}
          style={styles.input}
        />
      </div>

      <div style={styles.inputGroup}>
        <label style={styles.label}>Home Club:</label>
        <span style={styles.spanText}>
          {teams.find((club) => club.id === clubId.id)?.name || "Unknown Club"}
        </span>
      </div>

      <div style={styles.inputGroup}>
        <label style={styles.label}>Visitor Club:</label>
        <select
          name="clubVisitor_id"
          value={formData.clubVisitor_id}
          onChange={handleInputChange}
          style={styles.input}
        >
          {teams
            .filter((club) => club.id !== formData.clubHome_id) // Vyloučí domácí tým
            .map((club) => (
              <option key={club.id} value={club.id}>
                {club.name}
              </option>
            ))}
        </select>
      </div>

      <div style={styles.modalActions}>
        <button onClick={handleAddSubmit} style={styles.primaryButton}>
          Create
        </button>
        <button
          onClick={() => setIsAddModalOpen(false)}
          style={styles.cancelButton}
        >
          Cancel
        </button>
      </div>
    </div>
  </div>
)}

    </div>
  );
};

export default MatchSchedule;

const styles = {
modalOverlay: {
    position: "fixed",
    top: 0,
    left: 0,
    width: "100%",
    height: "100%",
    backgroundColor: "rgba(0, 0, 0, 0.5)",
    display: "flex",
    justifyContent: "center",
    alignItems: "center",
    zIndex: 1000,
    },
    modalContent: {
    backgroundColor: "#fff",
    padding: "30px",
    borderRadius: "10px",
    boxShadow: "0 4px 10px rgba(0, 0, 0, 0.2)",
    width: "400px",
    display: "flex",
    flexDirection: "column",
    gap: "15px",
    },
    modalTitle: {
    margin: "0 0 10px",
    textAlign: "center",
    fontSize: "24px",
    fontWeight: "bold",
    color: "#333",
    },
    inputGroup: {
    display: "flex",
    flexDirection: "column",
    gap: "5px",
    },
    label: {
    fontWeight: "bold",
    color: "#555",
    fontSize: "14px",
    },
    input: {
    padding: "8px",
    border: "1px solid #ccc",
    borderRadius: "5px",
    fontSize: "14px",
    },
    spanText: {
    backgroundColor: "#f0f0f0",
    padding: "8px",
    borderRadius: "5px",
    fontSize: "14px",
    color: "#333",
    },
    modalActions: {
    display: "flex",
    justifyContent: "space-between",
    marginTop: "10px",
    },
    primaryButton: {
    backgroundColor: "limegreen",
    color: "white",
    border: "none",
    padding: "10px 15px",
    borderRadius: "5px",
    cursor: "pointer",
    fontWeight: "bold",
    fontSize: "14px",
    },
    cancelButton: {
    backgroundColor: "red",
    color: "white",
    border: "none",
    padding: "10px 15px",
    borderRadius: "5px",
    cursor: "pointer",
    fontWeight: "bold",
    fontSize: "14px",
    },
  container: {
    width: "90%",
    margin: "0 auto",
    padding: "20px",
    backgroundColor: "#fff",
    borderRadius: "10px",
    boxShadow: "0 4px 8px rgba(0,0,0,0.2)",
    fontFamily: "Arial, sans-serif",
  },
  header: {
    textAlign: "center",
    fontSize: "28px",
    marginBottom: "20px",
    color: "#333",
  },
  addButton: {
    backgroundColor: "limegreen",
    color: "black",
    border: "none",
    padding: "10px 15px",
    borderRadius: "5px",
    fontWeight: "bold",
    cursor: "pointer",
    marginBottom: "10px",
  },
  table: {
    width: "100%",
    borderCollapse: "collapse",
  },
  tableHeader: {
    backgroundColor: "#4a4a4a",
    color: "#fff",
    padding: "10px",
    textAlign: "center",
  },
  tableCell: {
    border: "1px solid #ccc",
    padding: "10px",
    textAlign: "center",
    backgroundColor: "#f9f9f9",
  },

  editButton: {
    backgroundColor: "blue",
    color: "white",
    border: "none",
    padding: "10px 15px",
    borderRadius: "5px",
    fontWeight: "bold",
    cursor: "pointer",
    marginBottom: "10px",
  },
  deleteButton: {
    backgroundColor: "red",
    color: "white",
    border: "none",
    padding: "10px 15px",
    borderRadius: "5px",
    cursor: "pointer",
    fontWeight: "bold",
    fontSize: "14px",
  },
};
