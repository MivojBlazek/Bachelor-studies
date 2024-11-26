import React from 'react';
import axiosClient from '../../axiosClient';
import P from './P';

export default function RefereeList({ game, feedbacks, setFeedbacks, error, setError, isMe = true }) {
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
        <div style={{ margin: '0px 10px 20px 10px' }}>
            {game.controls.length > 0 ? (
                <>
                    <div style={{
                        alignSelf: 'center',
                        justifyContent: 'center',
                        margin: '0px auto',
                        maxWidth: '500px',
                        display: 'grid',
                        gridTemplateColumns: '12px 3fr 1fr 8fr',
                        gridAutoFlow: 'row',
                        gap: '20px',
                        textAlign: 'left',
                    }}>
                        <span></span>
                        <h3 style={{ marginBottom: '0px' }}>Referees:</h3>
                        <span></span>
                        <span></span>

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
                                    placeholder={isMe && 'Insert feedback... '}
                                    value={feedbacks[control.id]}
                                    onChange={(e) => feedbackChange(control.id, e.target.value)}
                                    onBlur={() => losingFocus(control.id)}
                                    onFocus={(e) => resizeTextarea(e.target)}
                                    onInput={(e) => resizeTextarea(e.target)}
                                    readOnly={!isMe}
                                    style={{
                                        padding: '8px',
                                        minHeight: '15px',
                                        resize: 'none',
                                        overflow: 'hidden',
                                        width: '100%',
                                        boxSizing: 'border-box',
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
