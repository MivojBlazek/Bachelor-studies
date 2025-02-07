<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Add Post</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f9f9f9;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }

        .header a {
            font-size: 20px;
            margin-bottom: 15px;
            padding: 10px;
            font-size: 14px;
            border: 1px solid #ccc;
            border-radius: 4px;
            background-color: #6a0dad;
            color: white;
            text-decoration: none;
        }

        .header h1 {
            font-size: 24px;
            color: #262626;
        }

        .header form {
            margin: 0;
        }

        .form-container {
            background: white;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 6px rgba(0, 0, 0, 0.2);
            width: 100%;
            max-width: 400px;
        }

        .form-container h1 {
            font-size: 24px;
            margin-bottom: 20px;
            color: #333;
            text-align: center;
        }

        .form-container form {
            display: flex;
            flex-direction: column;
        }

        .form-container input,
        .form-container textarea,
        .form-container button,
        .form-container select {
            margin-bottom: 15px;
            padding: 10px;
            font-size: 14px;
            border: 1px solid #ccc;
            border-radius: 4px;
        }

        .form-container button {
            background-color: #6a0dad;
            color: white;
            border: none;
            font-weight: bold;
            cursor: pointer;
            transition: background-color 0.3s;
        }

        .form-container button:hover {
            background-color: #520b99;
        }

        .form-container textarea {
            resize: none;
        }

        /* Modal styling */
        .modal {
            display: none;
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(0, 0, 0, 0.5);
            justify-content: center;
            align-items: center;
        }

        .modal-content {
            background: white;
            padding: 20px;
            border-radius: 8px;
            width: 90%;
            max-width: 400px;
            position: relative;
        }

        .modal-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .modal-header h2 {
            margin: 0;
            font-size: 20px;
        }

        .close-modal {
            background: none;
            border: none;
            font-size: 24px;
            cursor: pointer;
        }

        .modal-body {
            margin-top: 20px;
        }

        .checkbox-group {
            display: flex;
            flex-direction: column;
        }

        .checkbox-group label {
            margin-bottom: 5px;
        }
    </style>
</head>
<body>

    <div class="form-container">
        <div class="header">
            <a href="{{ route('FYP') }}">Back</a>
            <h1>Add Post</h1>   
        </div>
        <form action="{{ route('addNewPost') }}" method="POST">
            @csrf
            <!-- Photo URL -->
            <label for="photo">Photo URL:</label>
            <input type="text" id="photo" name="photo" placeholder="Enter photo URL" required>

            <!-- Description -->
            <label for="description">Description:</label>
            <textarea id="description" name="description" rows="3" placeholder="Enter description (optional)"></textarea>

            <!-- Visibility -->
            <label for="visibility">Visibility:</label>
            <select id="visibility" name="visibility" required onchange="toggleUserSelection()">
                <option value="public" selected>Public</option>
                <option value="private">Private</option>
            </select>

            <!-- Modal trigger for private post -->
            <button type="button" id="selectUsersBtn" style="display: none;">Choose Users</button>

            <!-- Place -->
            <label for="place">Place:</label>
            <input type="text" id="place" name="place" placeholder="Enter location (optional)">

            <!-- Submit Button -->
            <button type="submit">Post</button>
        </form>
    </div>

    <!-- Modal for user selection -->
    <div class="modal" id="userModal">
        <div class="modal-content">
            <div class="modal-header">
                <h2>Select Users</h2>
                <button class="close-modal" onclick="toggleModal()">×</button>
            </div>
            <div class="modal-body">
                <div class="checkbox-group">
                    @foreach ($users as $user)
                        <label>
                            <input type="checkbox" name="users[]" value="{{ $user->userId }}">
                            {{ $user->username }}
                        </label>
                    @endforeach
                </div>
                <button type="button" onclick="toggleModal()">Done</button>
            </div>
        </div>
    </div>

    <script>
        // Show or hide the user selection modal
        function toggleModal() {
            const modal = document.getElementById('userModal');
            modal.style.display = modal.style.display === 'flex' ? 'none' : 'flex';
        }

        // Show or hide the 'Choose Users' button based on visibility selection
        function toggleUserSelection() {
            const visibility = document.getElementById('visibility').value;
            const selectUsersBtn = document.getElementById('selectUsersBtn');

            if (visibility === 'private') {
                selectUsersBtn.style.display = 'block';
            } else {
                selectUsersBtn.style.display = 'none';
                document.getElementById('userModal').style.display = 'none';
            }
        }

        // Event listener for the 'Choose Users' button
        document.getElementById('selectUsersBtn').addEventListener('click', toggleModal);

        // Ensure modal is hidden initially
        document.addEventListener('DOMContentLoaded', toggleUserSelection);
    </script>
</body>
</html>
