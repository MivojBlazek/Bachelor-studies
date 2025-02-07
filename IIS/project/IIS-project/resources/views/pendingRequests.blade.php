<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Pending Requests</title>
    <style>
        body, html {
            margin: 0;
            padding: 0;
            font-family: Arial, sans-serif;
            height: 100%;
            background-color: #f3f4f6;
            color: #333;
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        h1 {
            font-size: 24px;
            margin: 20px 0;
            color: #222;
        }

        .user-list {
            width: 100%;
            max-width: 600px;
            margin: 20px auto;
            display: flex;
            flex-direction: column;
            gap: 15px;
        }

        .user-list div {
            background-color: #fff;
            border-radius: 8px;
            padding: 15px;
            display: flex;
            align-items: center;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        }
        
        .user-link {
            display: flex;
            align-items: center;
            text-decoration: none;
            color: inherit;
            width: 100%;
        }

        .user-photo {
            width: 50px;
            height: 50px;
            border-radius: 50%;
            object-fit: cover;
            margin-right: 15px;
            border: 2px solid #ddd;
        }

        .user-info {
            flex-grow: 1;
        }

        .user-name {
            font-size: 18px;
            margin: 0 0 5px;
            color: #333;
        }

        form {
            margin-left: auto;
        }

        button {
            padding: 8px 16px;
            background-color: #3897f0;
            color: #fff;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            font-size: 14px;
            font-weight: bold;
        }

        button.refuse {
            padding: 8px 16px;
            background-color: #ff3333;
            color: #fff;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            font-size: 14px;
            font-weight: bold;
        }

    </style>
</head>
<body>
    <h1>Pending Requests</h1>

    <div class="user-list">
        @foreach($pendingUsers as $pendingUser)
            <div>
                <a href="{{ route('profile', ['username' => $pendingUser->username]) }}" class="user-link">
                    <img src="{{ $pendingUser->profile_photo }}" alt="User" class="user-photo">
                    <div class="user-info">
                        <h4 class="user-name">{{ $pendingUser->name }}</h4>
                    </div>
                    <form action="{{ route('groups.approve') }}" method="POST">
                        @csrf
                        <input type="hidden" name="groupId" value="{{ $group->groupId }}">
                        <input type="hidden" name="userId" value="{{ $pendingUser->userId }}">
                        <button type="submit">Approve</button>
                    </form>
                    <form action="{{ route('groups.refuse') }}" method="POST">
                        @csrf
                        <input type="hidden" name="groupId" value="{{ $group->groupId }}">
                        <input type="hidden" name="userId" value="{{ $pendingUser->userId }}">
                        <button type="submit" class="refuse">Refuse</button>
                    </form>
                </a>
            </div>
        @endforeach
    </div>
</body>
</html>
