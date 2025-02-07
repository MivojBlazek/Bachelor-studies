<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>GroupWall</title>
    <style>
        /* Resetting some default styling */
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: Arial, sans-serif;
        }

        /* Body styling for full viewport height and centered content */
        body, html {
            height: 100%;
            display: flex;
            flex-direction: column;
            align-items: center;
            background-color: #fafafa;
        }

        /* Header styling similar to Instagram’s top navigation bar */
        .header {
            width: 100%;
            max-width: 800px;
            padding: 10px 20px;
            background-color: white;
            display: flex;
            justify-content: space-between;
            align-items: center;
            border-bottom: 1px solid #dbdbdb;
            position: sticky;
            top: 0;
            z-index: 1;
        }

        .header h1 {
            font-size: 24px;
            color: #262626;
        }

        .header form {
            margin: 0;
        }

        /* Main container styling */
        .container {
            width: 100%;
            max-width: 600px;
            margin-top: 20px;
        }

        /* Post layout styling */
        .post {
            background-color: white;
            border: 1px solid #dbdbdb;
            border-radius: 8px;
            margin-bottom: 20px;
            overflow: hidden;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }

        /* User info section above each post */
        .user-info {
            height: 45px;
            max-height: 45px;
            display: flex;
            align-items: center;
            padding: 5px 15px;
        }

        /* Profile photo styling */
        .profile-photo {
            width: 35px;
            height: 35px;
            max-width: 35px;
            max-height: 35px;
            border-radius: 50%;
            object-fit: fill;
            margin-right: 10px;
        }

        /* Username styling */
        .username {
            font-size: 14px;
            font-weight: 600;
            color: #262626;
        }

        .post img {
            width: 100%;
            height: auto;
            display: block;
        }

        .post-content {
            padding: 10px 15px;
        }

        .post-content h2 {
            font-size: 16px;
            font-weight: 600;
            color: #262626;
            margin: 10px 0 5px;
        }

        .post-content p {
            font-size: 14px;
            color: #8e8e8e;
            margin-bottom: 5px;
        }

        .like-comment-section {
            font-size: 14px;
            color: #262626;
            font-weight: 500;
            margin-top: 10px;
        }

        /* Comments styling */
        .comment {
            font-size: 13px;
            color: #262626;
            padding: 5px 0;
            border-top: 1px solid #efefef;
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
    <!-- Header Section -->
    <div class="header">
        <h1>GroupWall</h1>
        <form method="POST" action="{{ route('logout') }}">
            @csrf
            <button type="submit" style="background: none; border: none; color: #3897f0; font-weight: bold;">Logout</button>
        </form>
    </div>

    <!-- Sorting Options -->
    <div class="sort-options">
        <form method="GET" action="{{ route('groups.wall', ['groupId' => $groupId]) }}">
            <label for="sortBy">Sort By:</label>
            <select name="sortBy" id="sortBy" onchange="this.form.submit()">
                <option value="newest" {{ request('sortBy') == 'newest' ? 'selected' : '' }}>Newest</option>
                <option value="oldest" {{ request('sortBy') == 'oldest' ? 'selected' : '' }}>Oldest</option>
                <option value="most-likes" {{ request('sortBy') == 'most-likes' ? 'selected' : '' }}>Most Likes</option>
                <option value="least-likes" {{ request('sortBy') == 'least-likes' ? 'selected' : '' }}>Least Likes</option>
            </select>
        </form>
    </div>

    <!-- Main Container -->
    <div class="container">
        @foreach($posts as $post)
            <!-- Post -->
            <div class="post">
                <!-- User Info with Profile Photo and Username -->
                <a href="{{ route('profile', ['username' => $post->creator->username]) }}" class="user-link">
                    <div class="user-info">
                        <img class="profile-photo" src="{{ $post->creator->profile_photo }}" alt="Profile Photo">
                        <span class="username">{{ $post->creator->username }}</span>
                        <!-- Ban user -->
                        @if(($user->rights == 'admin' || $user->rights == 'moderator') &&
                            (!$post->creator->banned_until || $post->creator->banned_until->isPast()) &&
                            ($post->creator->rights == 'user'))
                            <form action="{{ route('profile.banUser') }}" method="POST">
                                @csrf
                                <input type="hidden" name="userId" value="{{ $post->creator->userId }}">
                                <button type="submit" style="padding: 15px; background: none; border: none; color: red; font-weight: bold; cursor: pointer;">Ban</button>
                            </form>
                        @endif
                        <!-- Remove post -->
                        @if($user->rights == 'admin' || $user->rights == 'moderator')
                            <form action="{{ route('FYP.removePost') }}" method="POST">
                                @csrf
                                <input type="hidden" name="postId" value="{{ $post->postId }}">
                                <button type="submit" style="padding: 15px; background: none; border: none; color: red; font-weight: bold; cursor: pointer;">Remove Post</button>
                            </form>
                        @endif
                    </div>
                </a>
                <!-- Image -->
                <img src="{{ $post->photo }}" alt="Post Photo">
                
                <!-- Post Content -->
                <div class="post-content">
                    <p class="like-comment-section">{{ $post->likes->count() }} Likes</p>
                    <form action="{{ route('FYP.like') }}" method="POST" style="display: inline;">
                        @csrf
                        <input type="hidden" name="postId" value="{{ $post->postId }}">
                        <button type="submit" style="background: none; border: none; color: #3897f0; font-weight: bold;">
                            @if($post->likes()->where('userId', $user->userId)->exists())
                                Unlike
                            @else
                                Like
                            @endif
                        </button>
                    </form>
                    <h2>{{ $post->description }}</h2>
                    <p>Posted by: {{ $post->creator->name }}</p>
                    
                    <!-- Comments Section -->
                    <form action="{{ route('FYP.addComment') }}" method="POST">
                        @csrf
                        <input type="hidden" name="postId" value="{{ $post->postId }}">
                        <input type="text" name="content" placeholder="Write comment..." />
                        <button type="submit">Add Comment</button>
                    </form>
                    @foreach($post->comments as $comment)
                        <div class="comment">
                            <p><strong>{{ $comment->user->name }}:</strong> {{ $comment->content }}</p>
                            <!-- Remove comment -->
                            @if($user->rights == 'admin' || $user->rights == 'moderator' || $user->userId == $comment->user->userId)
                                <form action="{{ route('FYP.removeComment') }}" method="POST">
                                    @csrf
                                    <input type="hidden" name="commentId" value="{{ $comment->commentId }}">
                                    <button type="submit" style="padding: 15px; background: none; border: none; color: red; font-weight: bold; cursor: pointer;">Remove Comment</button>
                                </form>
                            @endif
                        </div>
                    @endforeach
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
