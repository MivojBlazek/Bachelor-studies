import React from 'react';
import axiosClient from '../../axiosClient';
import P from './P';

export default function RefereeList({ game, feedbacks, setFeedbacks, error, setError }) {
    const feedbackChange = (controlId, newFeedback) => {
        setFeedbacks((prevFeedbacks) => ({
            ...prevFeedbacks,
            [controlId]: newFeedback,
        }));
    };

    const losingFocus = async (controlId) => {
        try
        {
            const feedbackToSubmit = feedbacks[controlId] || '';
            await axiosClient.post(`/delegate/controls`, { feedback: feedbackToSubmit, controlId });
        }
        catch (error)
        {
            setError(error.message);
        }
    };

    const resizeTextarea = (textarea) => {
        if (textarea)
        {
            textarea.style.height = 'auto';
            textarea.style.height = `${textarea.scrollHeight}px`;
        }
    };

    return (
        <div style={{ marginBottom: '20px' }}>
            <h3>Referees:</h3>
            {game.controls.length > 0 ? (
                <>
                    <div style={{
                        alignSelf: 'center',
                        justifyContent: 'center',
                        margin: '0px auto',
                        maxWidth: '800px',
                        display: 'grid',
                        gridTemplateColumns: '12px auto auto auto',
                        gridAutoFlow: 'row',
                        gap: '20px',
                        textAlign: 'left',
                    }}>
                        <span></span>
                        <span style={{ fontWeight: 'bold' }}>Name</span>
                        <span style={{ fontWeight: 'bold' }}>Role</span>
                        <span style={{ fontWeight: 'bold' }}>Feedback</span>

                        {game.controls.map(control => (
                            <React.Fragment key={control.id}>
                                <span
                                    style={{
                                        content: '•',
                                        fontSize: '30px',
                                        color: 'black',
                                        alignItems: 'center',
                                    }}
                                >•</span>
                                <P
                                    label={`${control.referee.name}`}
                                    href={`/delegate/referee_profile/${control.referee.id}`}
                                    style={{ padding: '8px 0px' }}
                                />
                                <span style={{ padding: '8px 0px' }}>{control.refereeRole}</span>
                                <textarea
                                    placeholder='Feedback'
                                    value={feedbacks[control.id]}
                                    onChange={(e) => feedbackChange(control.id, e.target.value)}
                                    onBlur={() => losingFocus(control.id)}
                                    onFocus={(e) => resizeTextarea(e.target)}
                                    onInput={(e) => resizeTextarea(e.target)}
                                    style={{
                                        padding: '8px',
                                        minWidth: '250px',
                                        maxWidth: '250px',
                                        minHeight: '15px',
                                        resize: 'none',
                                        overflow: 'hidden',
                                    }}
                                />
                            </React.Fragment>
                        ))}
                    </div>
                </>
            ) : (
                <p>---</p>
            )}
        </div>
    );
}
