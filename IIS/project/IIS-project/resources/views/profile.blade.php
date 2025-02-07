<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{{ $viewedUser->username }}'s Profile</title>
    <style>
        /* Basic styling for the profile page */
        body {
            font-family: Arial, sans-serif;
            background-color: #fafafa;
            display: flex;
            align-items: center;
            flex-direction: column;
            padding: 20px;
        }

        .profile-header {
            display: flex;
            align-items: center;
            margin-bottom: 20px;
            width: 100%;
            max-width: 800px;
            padding: 10px;
            background-color: white;
            border: 1px solid #dbdbdb;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }

        .profile-photo {
            width: 100px;
            height: 100px;
            border-radius: 50%;
            margin-right: 20px;
            object-fit: cover;
        }

        .profile-info {
            flex: 1;
        }

        .profile-info h1 {
            font-size: 24px;
            margin-bottom: 5px;
            color: #262626;
        }

        .profile-info p {
            font-size: 16px;
            color: #8e8e8e;
        }

        .post-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));
            gap: 20px;
            width: 100%;
            max-width: 800px;
            margin-top: 20px;
        }

        .post {
            background-color: white;
            border: 1px solid #dbdbdb;
            border-radius: 8px;
            overflow: hidden;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }

        .post img {
            width: 100%;
            height: auto;
            display: block;
        }

        .post-content {
            padding: 10px;
            font-size: 14px;
            color: #262626;
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
    <!-- Profile Header -->
    <div class="profile-header">
        <img class="profile-photo" src="{{ $viewedUser->profile_photo }}" alt="{{ $viewedUser->username }}'s Profile Photo">
        <div class="profile-info">
            <h1>
                {{ $viewedUser->username }}
                @if($viewedUser->username === $user->username)
                    <!-- Show Edit Button if the authenticated user owns the profile -->
                    <a href="{{ route('profile.edit', ['username' => $user->username]) }}" 
                    style="font-size: 14px; margin-left: 10px; text-decoration: none; color: #3897f0;">
                        Edit
                    </a>
                @endif

                <!-- User banning functionality -->
                @if(($user->rights == 'admin' || $user->rights == 'moderator') &&
                    (!$viewedUser->banned_until || $viewedUser->banned_until->isPast()) &&
                    ($viewedUser->rights == 'user'))
                    <form action="{{ route('profile.banUser') }}" method="POST">
                        @csrf
                        <input type="hidden" name="userId" value="{{ $viewedUser->userId }}">
                        <button type="submit" style="padding: 5px; background: none; border: none; color: red; font-weight: bold; cursor: pointer;">Ban</button>
                    </form>
                @endif

                <!-- User removing functionality -->
                @if(($user->rights == 'admin') && ($viewedUser->rights == 'user'))
                    <form action="{{ route('profile.removeUser') }}" method="POST">
                        @csrf
                        <input type="hidden" name="userId" value="{{ $viewedUser->userId }}">
                        <button type="submit" style="padding: 5px; background: none; border: none; color: red; font-weight: bold; cursor: pointer;">Remove</button>
                    </form>
                @endif
            </h1>
            <p>{{ $viewedUser->bio ?? 'No bio available' }}</p>
        </div>
    </div>

    <!-- User Posts -->
    <div class="post-grid">
        @foreach($posts as $post)
            <div class="post">
                <a href="{{ route('profile.post.show', $post->postId) }}">
                    <img src="{{ $post->photo }}" alt="Post Photo">
                </a>
                <div class="post-content">
                    <p>{{ $post->description }}</p>
                    <p>{{ $post->likes->count() }} Likes</p>
                </div>
            </div>
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