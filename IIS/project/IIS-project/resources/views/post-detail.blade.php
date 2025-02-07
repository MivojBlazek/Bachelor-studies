<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>FYP</title>
    <style>
        /* Resetting some default styling */
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: Arial, sans-serif;
        }

        body, html {
            height: 100%;
            display: flex;
            flex-direction: column;
            align-items: center;
            background-color: #fafafa;
        }

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
    </style>
</head>
<body>
    <!-- Header Section -->
    <div class="header">
        <h1>POST</h1>
        <a href="{{ route('profile', ['username' => $post->creator->username]) }}">Back</a>
        <form method="POST" action="{{ route('logout') }}">
            @csrf
            <button type="submit" style="background: none; border: none; color: #3897f0; font-weight: bold;">Logout</button>
        </form>
    </div>

    <!-- Main Container -->
    <div class="container">
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
                    @if($user->rights == 'admin' || $user->rights == 'moderator' || $user->userId == $post->creator->userId)
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
                
                @if($user->rights == 'admin' || $user->rights == 'moderator' || $user->userId == $post->creator->userId)
                    <form action="{{ route('profile.updateDescription') }}" method="POST">
                        @csrf
                        <input type="hidden" name="postId" value="{{ $post->postId }}">
                        <textarea name="description" rows="4" style="width: 100%; font-size: 15px; font-weight: bold;">{{ $post->description }}</textarea>
                        <button type="submit">Save Changes</button>
                    </form>
                @else
                    <h2>{{ $post->description }}</h2>
                @endif
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
    </div>
</body>
</html>