/**
 * ITU project
 * 
 * Author: Matyáš Sapík <xsapik02>
 */
import { Navigate } from "react-router-dom";
import { useStateContext } from "../../contexts/contextprovider";
import axiosClient from "../../axiosClient";
import { useEffect, useState } from "react";

export default function Profile() {
    const { user, token, setUser } = useStateContext();
    const [players, setPlayers] = useState([]); // State for players
    const [errors, setErrors] = useState(null); // State for errors
    const [selectedPlayer, setSelectedPlayer] = useState(null); // State for selected player
    const [isPopupOpen, setIsPopupOpen] = useState(false); // State for popup visibility

    if (!token) {
        return <Navigate to="/login" />;
    }

    useEffect(() => {
        const fetchPlayer = async () => {
            try {
                const { data } = await axiosClient.get(`/club/player/playershow`);
                setPlayers(data.players); // Update the players state
            } catch (err) {
                setErrors({ message: 'Error fetching data or club not found' });
            }
        };

        fetchPlayer();
    }, []);

    const groupByRole = (players) => {
        const rolesOrder = ['new', 'goalkeeper', 'defend', 'forward']; // Define the desired order

        // Group players by role
        const grouped = players.reduce((grouped, player) => {
            (grouped[player.role] = grouped[player.role] || []).push(player);
            return grouped;
        }, {});

        // Sort roles based on the predefined order
        const sortedGrouped = {};
        rolesOrder.forEach((role) => {
            if (grouped[role]) {
                sortedGrouped[role] = grouped[role];
            }
        });

        return sortedGrouped;
    };

    const handleCardClick = (player) => {
        setSelectedPlayer(player);
        setIsPopupOpen(true);
    };

    const handleSave = async () => {
        try {
            if (selectedPlayer.id) {
                // Update existing player
                await axiosClient.put(`/club/player/${selectedPlayer.id}`, selectedPlayer);
                setPlayers((prev) =>
                    prev.map((p) => (p.id === selectedPlayer.id ? selectedPlayer : p))
                );
                setIsPopupOpen(false);
            }
        } catch (err) {
            setErrors({ message: 'Error saving player' });
        }
    };

    const handleDelete = async () => {
        try {
            if (selectedPlayer.id) {
                // Delete existing player
                await axiosClient.delete(`/club/player/${selectedPlayer.id}`);
                setPlayers((prev) => prev.filter((p) => p.id !== selectedPlayer.id));
                setIsPopupOpen(false);
            }
        } catch (err) {
            setErrors({ message: 'Error deleting player' });
        }
    };

    const handleAddPlayer = async () => {
        try {
            const newPlayer = {
                name: "New Player",
                dress_number: 1,
                role: "new", // Default role
                address: "",
                phone: "",
                email: "",
            };

            // Send request to create a new player
            const { data } = await axiosClient.post(`/club/player/playercreate`, newPlayer);

            // Add the new player to the state
            setPlayers((prev) => [...prev, data.player]);
        } catch (err) {
            setErrors({ message: 'Error adding player' });
        }
    };

    const groupedPlayers = groupByRole(players);

    return (
        <div style={{ display: "flex", justifyContent: "center"}}>
            <div style={{ width: "800px", textAlign: "center", backgroundColor: "#D9D9D9" }}> {/* Fixed width container */}
                <h1 style={{ color: "green" }}>Roster</h1>

                {/* Display errors if any */}
                {errors && <p style={{ color: "red" }}>{errors.message}</p>}

                <div style={{ display: "flex", justifyContent: "flex-end", margin: "10px" }}>
                    <button
                        onClick={handleAddPlayer}
                        style={{
                            backgroundColor: "green",
                            color: "white",
                            padding: "10px",
                            border: "none",
                            borderRadius: "5px",
                        }}
                    >
                        Add Player
                    </button>
                </div>

                <div>
                    {Object.keys(groupedPlayers).length > 0 ? (
                        Object.keys(groupedPlayers).map((role) => (
                            <div key={role} style={{ marginBottom: "30px" }}>
                                <h2>{role}</h2>
                                <div style={{ display: "flex", justifyContent: "center", flexWrap: "wrap", gap: "20px" }}>
                                    {groupedPlayers[role].map((player) => (
                                        <div
                                            key={player.id}
                                            onClick={() => handleCardClick(player)}
                                            style={{
                                                width: "200px",
                                                backgroundColor: "#536882",
                                                color: "white",
                                                padding: "15px",
                                                borderRadius: "10px",
                                                textAlign: "center",
                                                cursor: "pointer",
                                            }}
                                        >
                                            <p style={{ fontWeight: "bold" }}>{player.name}</p>
                                            <p>{player.dress_number}</p>
                                            <p>{player.address}</p>
                                            <p>{player.phone}</p>
                                            <p>{player.email}</p>
                                            <p style={{ fontWeight: "bold" }}>{player.role}</p>
                                        </div>
                                    ))}
                                </div>
                            </div>
                        ))
                    ) : (
                        <p>No players found for this club.</p>
                    )}
                </div>

                {isPopupOpen && selectedPlayer && (
                    <div
                        style={{
                            position: "fixed",
                            top: "50%",
                            left: "50%",
                            transform: "translate(-50%, -50%)",
                            backgroundColor: "white",
                            padding: "20px",
                            borderRadius: "10px",
                            boxShadow: "0 4px 8px rgba(0, 0, 0, 0.2)",
                            zIndex: 1000,
                        }}
                    >
                        <h2>Edit Player</h2>
                        <form
                            onSubmit={(e) => {
                                e.preventDefault();
                                handleSave();
                            }}
                        >
                            <div style={{ marginBottom: "10px" }}>
                                <label>Name: </label>
                                <input
                                    type="text"
                                    value={selectedPlayer.name}
                                    onChange={(e) =>
                                        setSelectedPlayer({ ...selectedPlayer, name: e.target.value })
                                    }
                                />
                            </div>
                            <div style={{ marginBottom: "10px" }}>
                                <label>Dress Number: </label>
                                <input
                                    type="number"
                                    value={selectedPlayer.dress_number}
                                    onChange={(e) =>
                                        setSelectedPlayer({ ...selectedPlayer, dress_number: e.target.value })
                                    }
                                />
                            </div>
                            <div style={{ marginBottom: "10px" }}>
                                <label>Role: </label>
                                <select
                                    value={selectedPlayer.role}
                                    onChange={(e) =>
                                        setSelectedPlayer({ ...selectedPlayer, role: e.target.value })
                                    }
                                    style={{ padding: "5px", borderRadius: "5px" }}
                                >
                                    <option value="forward">Forward</option>
                                    <option value="defend">Defend</option>
                                    <option value="goalkeeper">Goalkeeper</option>
                                </select>
                            </div>
                            <div style={{ marginBottom: "10px" }}>
                                <label>Address: </label>
                                <input
                                    type="text"
                                    value={selectedPlayer.address}
                                    onChange={(e) =>
                                        setSelectedPlayer({ ...selectedPlayer, address: e.target.value })
                                    }
                                />
                            </div>
                            <div style={{ marginBottom: "10px" }}>
                                <label>Phone: </label>
                                <input
                                    type="text"
                                    value={selectedPlayer.phone}
                                    onChange={(e) =>
                                        setSelectedPlayer({ ...selectedPlayer, phone: e.target.value })
                                    }
                                />
                            </div>
                            <div style={{ marginBottom: "10px" }}>
                                <label>Email: </label>
                                <input
                                    type="email"
                                    value={selectedPlayer.email}
                                    onChange={(e) =>
                                        setSelectedPlayer({ ...selectedPlayer, email: e.target.value })
                                    }
                                />
                            </div>
                            <button type="submit" style={{ backgroundColor: "green", color: "white", padding: "10px", border: "none", borderRadius: "5px" }}>Save</button>
                            <button
                                type="button"
                                onClick={() => setIsPopupOpen(false)}
                                style={{ marginLeft: "10px", padding: "10px", borderRadius: "5px" }}
                            >
                                Cancel
                            </button>
                            <button
                                type="button"
                                onClick={() => {
                                    if (window.confirm("Are you sure you want to delete this player")) {
                                        handleDelete();
                                    }}}
                                style={{ marginLeft: "10px", backgroundColor: "red", color: "white", padding: "10px", border: "none", borderRadius: "5px" }}
                            >
                                Delete</button>
                        </form>
                    </div>
                )}

                {isPopupOpen && (
                    <div
                        style={{
                            position: "fixed",
                            top: 0,
                            left: 0,
                            width: "100%",
                            height: "100%",
                            backgroundColor: "rgba(0, 0, 0, 0.5)",
                            zIndex: 999,
                        }}
                        onClick={() => setIsPopupOpen(false)}
                    />
                )}
            </div>
        </div>
    );
}
