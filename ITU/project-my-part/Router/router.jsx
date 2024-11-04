import {createBrowserRouter} from 'react-router-dom';
import Login from './views/login.jsx';
import DefaultLayout from './Layouts/DefaultLayout.jsx';
import GuestLayout from './Layouts/GuestLayout.jsx';
import ClubLayout from './Layouts/ClubLayout.jsx';
import DelegateLayout from './Layouts/DelegateLayout.jsx';
import RefereeLayout from './Layouts/RefereeLayout.jsx';
import { useStateContext } from "./contexts/contextprovider";

import Dashboard from './views/delegate/dashboard.jsx';
import GameList from './views/delegate/game_list.jsx';
import Profile from './views/delegate/profile.jsx';
import GameDetail from './views/delegate/game_detail.jsx';
import DelegateProfile from './views/delegate/delegate_profile.jsx';
import ClubProfile from './views/delegate/club_profile.jsx';
import RefereeProfile from './views/delegate/referee_profile.jsx';
import GameVideos from './views/delegate/game_videos.jsx';
import PaymentDetail from './views/delegate/payment_detail.jsx';
import Payments from './views/delegate/payments.jsx';

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

const router = createBrowserRouter ([
    {
        path: '/',
        element: <RoleBasedLayout />,
        children: [
            {
                path: 'delegate/dashboard',
                element: <Dashboard />,
            },
            {
                path: 'delegate/game_list',
                element: <GameList />,
            },
            {
                path: 'delegate/profile',
                element: <Profile />,
            },
            {
                path: 'delegate/allPayments',
                element: <Payments />,
            },
            {
                path: 'delegate/game_detail/:id',
                element: <GameDetail />,
            },
            {
                path: 'delegate/delegate_profile/:delegateId',
                element: <DelegateProfile />,
            },
            {
                path: 'delegate/club_profile/:clubId',
                element: <ClubProfile />,
            },
            {
                path: 'delegate/referee_profile/:refereeId',
                element: <RefereeProfile />,
            },
            {
                path: 'delegate/game/:id/videos',
                element: <GameVideos />,
            },
            {
                path: 'delegate/payment/:paymentId',
                element: <PaymentDetail />,
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