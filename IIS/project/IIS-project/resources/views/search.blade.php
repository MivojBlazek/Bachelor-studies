<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Search</title>
    <meta name="csrf-token" content="{{ csrf_token() }}">
    <style>
        body, html {
            height: 100%;
            display: flex;
            flex-direction: column;
            align-items: center;
            background-color: #fafafa;
        }

        .user-link {
            display: flex;
            align-items: center;
            text-decoration: none;
            color: inherit;
        }

        .user-photo {
            width: 50px;
            height: 50px;
            border-radius: 50%;
            object-fit: cover;
            margin-right: 15px;
        }
        
        .user-info {
            display: flex;
            flex-direction: column;
        }

        .user-name {
            font-size: 16px;
            font-weight: bold;
            margin: 0;
        }

        .user-bio {
            font-size: 14px;
            color: #8e8e8e;
        }

        .group-info {
            display: flex;
            flex-direction: column;
        }

        .group-info h4 {
            font-size: 16px;
            font-weight: bold;
            margin: 0;
        }

        .group-info p {
            font-size: 14px;
            color: #8e8e8e;
        }

        .search-options {
            display: flex;
            justify-content: center;
            margin-bottom: 20px;
            gap: 10px;
        }

        .search-option {
            padding: 10px 20px;
            background: white;
            border: 1px solid #ddd;
            border-radius: 5px;
            cursor: pointer;
            transition: background 0.3s;
        }

        .search-option.active {
            background: #6a0dad;
            color: white;
        }

        .search-bar {
            margin-bottom: 20px;
            text-align: center;
        }

        .search-bar input {
            width: 100%;
            max-width: 600px;
            padding: 10px;
            font-size: 16px;
            border: 1px solid #ddd;
            border-radius: 5px;
        }

        .results {
            display: flex;
            flex-direction: column;
            gap: 10px;
        }

        .result-item {
            display: flex;
            align-items: center;
            margin-bottom: 5px;
            border: 1px solid #dbdbdb;
            border-radius: 8px;
            padding: 10px;
            background-color: white;
        }

        .result-item img {
            width: 50px;
            height: 50px;
            border-radius: 50%;
            object-fit: cover;
        }

        .result-item h4 {
            margin: 0;
        }

        .result-item p {
            margin: 0;
            color: #666;
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
    <h1>Search</h1>

    <!-- Search Options -->
    <div class="search-options">
        <div class="search-option active" data-type="users">Users</div>
        <div class="search-option" data-type="groups">Groups</div>
        <div class="search-option" data-type="posts">Posts</div>
    </div>

    <!-- Search Bar -->
    <div class="search-bar">
        <input type="text" id="searchInput" placeholder="Type to search..." />
    </div>

    <!-- Results Container -->
    <div class="results" id="resultsContainer">
        <p>Type something to search.</p>
    </div>

    <script>
        const csrfToken = document.querySelector('meta[name="csrf-token"]').getAttribute('content');
        const searchOptions = document.querySelectorAll('.search-option');
        const searchInput = document.getElementById('searchInput');
        const resultsContainer = document.getElementById('resultsContainer');

        let activeType = 'users'; // Default search type

        // Change active search type
        searchOptions.forEach(option => {
            option.addEventListener('click', () => {
                searchOptions.forEach(opt => opt.classList.remove('active'));
                option.classList.add('active');
                activeType = option.dataset.type;
                handleSearch(); // Perform search when type changes
            });
        });

        // Handle input for search
        searchInput.addEventListener('input', handleSearch);

        function handleSearch() {
            const query = searchInput.value.trim();

            if (query === '') {
                resultsContainer.innerHTML = '<p>Type something to search.</p>';
                return;
            }

            const endpoint = activeType === 'users' 
                ? '{{ route('search.users') }}'
                : activeType === 'groups'
                ? '{{ route('search.groups') }}'
                : '{{ route('search.posts') }}';

            fetch(endpoint, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'X-CSRF-TOKEN': csrfToken
                },
                body: JSON.stringify({ name: query })
            })
            .then(response => response.json())
            .then(data => {
                let resultsArray = data;

                if (activeType === 'posts') {
                    resultsArray = data.posts || [];
                    console.log(resultsArray);
                    if (resultsArray.length === 0) {
                        resultsContainer.innerHTML = '<p>No results found!</p>';
                        return;
                    }
                }
                displayResults(resultsArray);
            })
            .catch(error => {
                console.error('Error:', error);
                resultsContainer.innerHTML = '<p>An error occurred while fetching results.</p>';
            });
        }

        function displayResults(data) {
            resultsContainer.innerHTML = '';

            if (data.length === 0) {
                resultsContainer.innerHTML = '<p>No results found.</p>';
                return;
            }

            data.forEach(item => {
                const resultItem = document.createElement('div');
                resultItem.classList.add('result-item');

                if (activeType === 'users') {
                    const profileLink = `/profile/${item.username || 'unknown-username'}`;
                    resultItem.innerHTML = `
                        <a href="${profileLink}" class="user-link">
                            <img src="${item.profile_photo || 'https://via.placeholder.com/50'}" alt="User" class="user-photo">
                            <div class="user-info">
                                <h4 class="user-name">${item.name}</h4>
                                <p class="user-bio">${item.bio || 'No bio available.'}</p>
                            </div>
                        </a>
                    `;
                } else if (activeType === 'groups') {
                    const isAdminOrModerator = '{{ $user->rights }}' === 'admin' || '{{ $user->rights }}' === 'moderator';
                    resultItem.innerHTML = `
                        ${isAdminOrModerator ? `<a href="/groups/${item.groupId}">` : ``}
                            <div class="group-info">
                                <h4>${item.name}</h4>
                                <p>Admin: ${item.user_as_admin.name}</p>
                                ${
                                    item.isInGroup
                                    ? `
                                    <form method="POST" action="{{ route('groups.leave') }}">
                                        <input type="hidden" name="_token" value="${csrfToken}">
                                        <input type="hidden" name="groupId" value="${item.groupId}">
                                        <button type="submit" class="leave-button">Leave Group</button>
                                    </form>
                                    `
                                    : `
                                    <form method="POST" action="{{ route('search.groups.join') }}">
                                        <input type="hidden" name="_token" value="${csrfToken}">
                                        <input type="hidden" name="groupId" value="${item.groupId}">
                                        <button type="submit" class="join-button">Join Group</button>
                                    </form>
                                    `
                                }
                                @if($user->rights == 'admin' || $user->rights == 'moderator')
                                    <form action="{{ route('search.groups.remove') }}" method="POST">
                                        <input type="hidden" name="_token" value="${csrfToken}">
                                        <input type="hidden" name="groupId" value="${item.groupId}">
                                        <button type="submit" style="padding: 5px; background: none; border: none; color: red; font-weight: bold; cursor: pointer;">Remove</button>
                                    </form>
                                @endif
                            </div>
                        ${isAdminOrModerator ? `</a>` : ``}
                    `;
                } else if (activeType === 'posts') {
                    const postLink = `/profile/post/${item.postId}`;
                    resultItem.innerHTML = `
                        <a href="${postLink}" class="post-link">
                            <img src="${item.photo || 'https://via.placeholder.com/50'}" alt="Post" class="post-photo">
                            <div class="post-info">
                                <p class="post-description">${item.description}</p>
                            </div>
                        </a>
                    `;
                }

                resultsContainer.appendChild(resultItem);
            });
        }
    </script>
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
