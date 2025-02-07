/**
 * ITU project
 * 
 * File: router.jsx
 * 
 * Authors: Michal Blažek <xblaze38>
 *          Matěj Lepeška <xlepes00>
 *          Matyáš Sapík  <xsapik02>
 */

import {createBrowserRouter} from 'react-router-dom';
import Login from './views/login.jsx';
import DefaultLayout from './Layouts/DefaultLayout.jsx';
import GuestLayout from './Layouts/GuestLayout.jsx';
import ClubLayout from './Layouts/ClubLayout.jsx';
import DelegateLayout from './Layouts/DelegateLayout.jsx';
import RefereeLayout from './Layouts/RefereeLayout.jsx';
import { useStateContext } from "./contexts/contextprovider";

import D_Dashboard from './views/delegate/dashboard.jsx';
import D_GameList from './views/delegate/game_list.jsx';
import D_Profile from './views/delegate/profile.jsx';
import D_GameDetail from './views/delegate/game_detail.jsx';
import D_DelegateProfile from './views/delegate/delegate_profile.jsx';
import D_ClubProfile from './views/delegate/club_profile.jsx';
import D_RefereeProfile from './views/delegate/referee_profile.jsx';
import D_GameVideos from './views/delegate/game_videos.jsx';
import D_PaymentDetail from './views/delegate/payment_detail.jsx';
import D_Payments from './views/delegate/payments.jsx';

import Dashboard from './views/referee/dashboard.jsx';
import RegisteredMatches from './views/referee/registered_matches.jsx';
import AllMatches from './views/referee/all_matches.jsx';
import GameDetail from './views/referee/game_detail.jsx';
import PlayedMatches from './views/referee/played_matches.jsx';
import Rating from './views/referee/rating.jsx'
import Payments from './views/referee/payments.jsx';
import Profile from './views/referee/profile.jsx';
import Video from './views/referee/video.jsx';
import PaymentEdit from './views/referee/payment_edit.jsx'

import ClubDashboard from './Components/club/dashboard.jsx';
import CProfile from './Components/club/profile.jsx';
import MatchSchedule from './Components/club/matchSchedule.jsx';
import Players from './Components/club/players.jsx';

/** Function uses layout according to user role */
function RoleBasedLayout()
{
    const { role } = useStateContext();

    return (
        <>
            {role === 'club' ? <ClubLayout /> :
            role === 'delegate' ? <DelegateLayout /> :
            role === 'referee' ? <RefereeLayout /> :
            <DefaultLayout />}
        </>
    );
}

/** Accessible routes for each role */
const router = createBrowserRouter ([
    {
        path: '/',
        element: <RoleBasedLayout />,
        children: [
            /** Delegate routes (Michal Blažek) */
            {
                path: 'delegate/dashboard',
                element: <D_Dashboard />,
            },
            {
                path: 'delegate/game_list',
                element: <D_GameList />,
            },
            {
                path: 'delegate/profile',
                element: <D_Profile />,
            },
            {
                path: 'delegate/allPayments',
                element: <D_Payments />,
            },
            {
                path: 'delegate/game_detail/:id',
                element: <D_GameDetail />,
            },
            {
                path: 'delegate/delegate_profile/:delegateId',
                element: <D_DelegateProfile />,
            },
            {
                path: 'delegate/club_profile/:clubId',
                element: <D_ClubProfile />,
            },
            {
                path: 'delegate/referee_profile/:refereeId',
                element: <D_RefereeProfile />,
            },
            {
                path: 'delegate/game/:id/videos',
                element: <D_GameVideos />,
            },
            {
                path: 'delegate/payment/:paymentId',
                element: <D_PaymentDetail />,
            },

            /** Referee routes (Matěj Lepeška) */
            {
                path: 'referee/dashboard',
                element: <Dashboard />,
            },
            {
                path: 'referee/profile',
                element: <Profile />,
            },
            {
                path: 'referee/all_matches',
                element: <AllMatches />,
            },
            {
                path: 'referee/registered_matches',
                element: <RegisteredMatches />,
            },
            {
                path: 'referee/played_matches',
                element: <PlayedMatches />,
            },
            {
                path: 'referee/played_matches/:matchId/rating',
                element: <Rating />,
            },
            {
                path: 'referee/all_matches/:id/game_detail',
                element: <GameDetail />,
            },
            {
                path: 'referee/payments',
                element: <Payments />,
            },
            {
                path: 'referee/payments/:controlId/payment_edit',
                element: <PaymentEdit />,
            },
            {
                path: 'referee/all_matches/videos',
                element: <Video />,
            },

            /** Club routes (Matyáš Sapík) */
            {
                path: 'club/dashboard', // This could be dynamic for each role
                element: <ClubDashboard /> // Just for example; adjust based on role
            },
            {
                path: 'club/profile',  // New route for the profile page
                element: <CProfile />,   // Profile component will render here
            },
            {
                path: 'club/matchSchedule',  // New route for the profile page
                element: <MatchSchedule />,   // Profile component will render here
            },
            {
                path: 'club/players',
                element: <Players />,
            },
        ]
    },

    {
        path: '/',
        element: <GuestLayout />,
        children: [
            {
                path: '/login',
                element: <Login />,
            },
        ]
    },
]);

export default router;