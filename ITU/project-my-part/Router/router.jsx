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
            //TODO

            /** Club routes (Matyáš Sapík) */
            //TODO
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