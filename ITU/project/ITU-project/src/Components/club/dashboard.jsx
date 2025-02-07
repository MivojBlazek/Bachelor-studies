/**
 * ITU project
 * 
 * Author: Matyáš Sapík <xsapik02>
 */
import React, { useState, useEffect } from "react";
import axiosClient from "../../axiosClient";

const Board = () => {
  const [events, setEvents] = useState([]); // State pro nadcházející události
  const [notifications, setNotifications] = useState(""); // State pro notifikace
  const [isLoading, setIsLoading] = useState(false); // Loading state
  const [errors, setErrors] = useState(null); // State pro chyby

  // Funkce pro načtení událostí
  const fetchBoardData = async () => {
    setIsLoading(true);
    try {
      // Načítání událostí
      const eventsResponse = await axiosClient.get("/club/board/upcoming");
      const allEvents = eventsResponse.data.data;

      // Filtrování pouze nadcházejících zápasů
      const upcomingEvents = allEvents.filter((event) => {
        const eventDate = new Date(event.date);
        const today = new Date();
        today.setHours(0, 0, 0, 0); // Resetujeme čas na začátek dne
        return eventDate >= today;
      });

      setEvents(upcomingEvents);

    } catch (err) {
      console.error("Error fetching data:", err);
      setErrors("Failed to load board data.");
    } finally {
      setIsLoading(false);
    }
  };

  useEffect(() => {
    fetchBoardData();
  }, []);

  return (
    <div style={styles.container}>
      {/* Nadcházející události */}
      <h1 style={styles.sectionTitle}>Upcoming events:</h1>
      {isLoading ? (
        <p style={styles.loading}>Loading events...</p>
      ) : errors ? (
        <p style={styles.error}>{errors}</p>
      ) : (
        <div style={styles.eventsGrid}>
          {events.map((event) => (
            <div key={event.id} style={styles.eventCard}>
              <p style={styles.eventDate}>
                {new Date(event.date).toLocaleDateString("en-GB", {
                  weekday: "short",
                  day: "2-digit",
                  month: "2-digit",
                  year: "numeric",
                })}
              </p>
              <p style={styles.eventMatch}>{event.league}</p>
              <p style={styles.eventDetails}>{event.location}</p>
            </div>
          ))}
        </div>
      )}

      {/* Notifikace */}
      <h1 style={styles.sectionTitle}>Notification:</h1>
      <div style={styles.notificationBox}>
        {notifications ? (
          <p>{notifications}</p>
        ) : (
          <p>No notifications at this time.</p>
        )}
      </div>
    </div>
  );
};

const styles = {
  container: {
    maxWidth: "1000px",
    margin: "20px auto",
    padding: "20px",
    backgroundColor: "#fff",
    borderRadius: "10px",
    boxShadow: "0 4px 8px rgba(0, 0, 0, 0.1)",
    fontFamily: "Arial, sans-serif",
  },
  header: {
    textAlign: "center",
    fontSize: "28px",
    marginBottom: "20px",
    color: "#333",
  },
  sectionTitle: {
    fontSize: "20px",
    marginBottom: "10px",
    color: "#555",
  },
  loading: {
    textAlign: "center",
    fontSize: "16px",
    color: "#777",
  },
  error: {
    textAlign: "center",
    fontSize: "16px",
    color: "red",
  },
  eventsGrid: {
    display: "grid",
    gridTemplateColumns: "repeat(auto-fill, minmax(200px, 1fr))",
    gap: "15px",
    marginBottom: "20px",
  },
  eventCard: {
    backgroundColor: "#E9E9E9",
    padding: "15px",
    borderRadius: "5px",
    textAlign: "center",
    boxShadow: "0 2px 4px rgba(0, 0, 0, 0.1)",
  },
  eventDate: {
    fontSize: "14px",
    fontWeight: "bold",
    marginBottom: "5px",
    color: "#555",
  },
  eventMatch: {
    fontSize: "16px",
    fontWeight: "bold",
    marginBottom: "5px",
    color: "#333",
  },
  eventDetails: {
    fontSize: "14px",
    color: "#777",
  },
  notificationBox: {
    backgroundColor: "#D9D9D9",
    padding: "15px",
    borderRadius: "5px",
    boxShadow: "0 2px 4px rgba(0, 0, 0, 0.1)",
    lineHeight: "1.5",
  },
};

export default Board;
