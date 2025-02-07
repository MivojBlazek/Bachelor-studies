<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My Groups</title>
    <style>
        /* Add basic styling for the groups page */
        /* Body styling for full viewport height and centered content */
        body, html {
            height: 100%;
            display: flex;
            flex-direction: column;
            align-items: center;
            background-color: #fafafa;
        }

        .group-list {
            display: flex;
            flex-direction: column;
            gap: 20px;
        }

        .group-item {
            display: flex;
            align-items: center;
            padding: 10px;
            border: 1px solid #dbdbdb;
            border-radius: 8px;
            background-color: white;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }

        .group-info {
            margin-left: 20px;
            display: flex;
            flex-direction: column;
        }

        .group-info h3 {
            font-size: 18px;
            margin: 0;
            color: #262626;
        }

        .group-info p {
            font-size: 14px;
            color: #8e8e8e;
        }

        .user-list {
            margin-top: 10px;
            font-size: 14px;
            color: #262626;
        }

        .user-list span {
            margin-right: 10px;
            font-weight: bold;
        }

        .bottom-nav {
            position: fixed;
            bottom: 0;
            width: 100%;
            max-width: 800px;
            display: flex;
            justify-content: space-around;
            align-items: center;
            background-color: white;
            border-top: 1px solid #dbdbdb;
            padding: 10px 0;
            z-index: 10;
            box-shadow: 0 -2px 4px rgba(0, 0, 0, 0.1); 
        }
        .bottom-nav a {
            text-decoration: none;
            color: #262626;
            font-size: 14px;
            font-weight: 500;
            text-align: center;
        }
        .bottom-nav a:hover {
            color: #3897f0;
        }
        .bottom-nav .icon {
            font-size: 24px;
            display: block;
            margin-bottom: 5px;
        }

    </style>
</head>
<body>
    <!-- Button Add group -->
    <div>
        <form action="{{ route('groups.addGroup') }}" method="POST">
            @csrf
            <input type="text" name="name" placeholder="Group name" />
            <button type="submit">Add group</button>
        </form>
    </div>

    <h1>My Groups</h1>

    <div class="group-list">
        @foreach($groups as $group)
            @if($group->isInGroup || $user->rights == 'admin' || $user->rights == 'moderator')
                <a href="{{ route('groups.wall', ['groupId' => $group->groupId]) }}">
                    <div class="group-item">
                        <div class="group-info">
                            <h3>{{ $group->name }}</h3>
                            <p>{{ $group->description ?? 'No description available.' }}</p>
                            <div class="user-list">
                                <span><strong>Admin:</strong> {{ $group->userAsAdmin->name ?? 'Unknown' }}</span>
                                <strong>Members:</strong>
                                @foreach($group->users as $member)
                                    <span>{{ $member->name }}</span>
                                @endforeach
                            </div>
                        </div>
                        <!-- Admin of group can approve members -->
                        @if($user->userId  == $group->userAsAdmin->userId)
                            <a href="{{ route('groups.pendingRequests', $group->groupId) }}">Pending Requests</a>
                        @endif
                        <!-- Remove group button -->
                        @if($user->rights == 'admin' || $user->rights == 'moderator' || $user->userId  == $group->userAsAdmin->userId)
                            <form action="{{ route('search.groups.remove') }}" method="POST">
                                @csrf
                                <input type="hidden" name="groupId" value="{{ $group->groupId }}">
                                <button type="submit" style="padding: 5px; background: none; border: none; color: red; font-weight: bold; cursor: pointer;">Remove</button>
                            </form>
                        @endif
                    </div>
                </a>
            @endif
        @endforeach
    </div>

    <!-- Bottom Navigation Bar -->
    <div class="bottom-nav">
        <a href="{{ route('FYP') }}">
            <span class="icon">🏠</span>
            FYP
        </a>
        <a href="{{ route('search') }}">
            <span class="icon">🔍</span>
            Search
        </a>
        <a href="{{ route('profile', ['username' => $user->username]) }}">
            <span class="icon">👤</span>
            My Profile
        </a>
        <a href="{{ route('myGroups') }}">
            <span class="icon">👥</span>
            Groups
        </a>
        <a href="{{ route('addPost') }}">
            <span class="icon">➕</span>
            Add Post
        </a>
    </div>
</body>
</html>
