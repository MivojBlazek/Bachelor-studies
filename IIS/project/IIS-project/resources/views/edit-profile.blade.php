<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{{ $user->username }}'s Profile</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background: #fff;
            color: black;
            line-height: 1.5;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
        }
        
        .profile-header {
            width: 100%;
            max-width: 500px;
            padding: 2rem;
            border: 1px solid #ccc;
            border-radius: 10px;
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
        }

        .profile-header a {
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

        .profile-info h1 {
            text-align: center;
            color: #6a0dad;
            margin-bottom: 1.5rem;
        }

        .profile-image {
            display: block;
            margin: 1rem auto;
            width: 250px;
            height: 250px;
            border-radius: 50%;
            object-fit: cover;
            border: 2px solid #6a0dad;
        }

        form {
            margin-bottom: 2rem;
        }

        label {
            display: block;
            font-weight: bold;
            margin-bottom: 0.5rem;
        }

        input[type="text"], input[type="email"], input[type="password"], input[type="number"],
        textarea {
            width: 100%;
            padding: 0.5rem;
            border: 1px solid #ccc;
            border-radius: 5px;
            margin-bottom: 1rem;
            font-size: 1rem;
        }

        textarea {
            resize: vertical;
            min-height: 100px;
        }

        button {
            display: block;
            width: 100%;
            padding: 0.7rem;
            background: #6a0dad;
            color: white;
            border: none;
            border-radius: 5px;
            font-size: 1rem;
            cursor: pointer;
        }

        button:hover {
            background: #5a0c9e;
        }

        .remove-account button {
            background: none;
            color: red;
            font-weight: bold;
            padding: 0.5rem;
            border: none;
            cursor: pointer;
        }

        .remove-account button:hover {
            text-decoration: underline;
        }

    </style>
</head>
<body>
    <div class="profile-header">
        <div class="profile-info">
            <a href="{{ route('profile', ['username' => $user->username]) }}">Back</a>
            <h1>{{ $user->username }}</h1>
        </div>

        <form action="{{ route('profile.editData') }}" method="POST">
            @csrf
            <label>Name</label>
            <input type="text" name="name" value="{{ $user->name }}" required>

            <label>Email</label>
            <input type="email" name="email" value="{{ $user->email }}" required>

            <label>Password</label>
            <input type="password" name="password" placeholder="Enter new password (optional)">

            <label>Confirm Password</label>
            <input type="password" name="password_confirmation" placeholder="Confirm new password">

            <label>Age</label>
            <input type="number" name="age" value="{{ $user->age ?? 0 }}" min="1" max="150">

            <label>Bio</label>
            <textarea name="bio">{{ $user->bio }}</textarea>

            <label>Profile Photo URL</label>
            <input type="text" name="profile_photo" value="{{ $user->profile_photo }}" required>
            <img src="{{ $user->profile_photo }}" alt="Profile Photo" class="profile-image">

            <button type="submit">Update</button>
        </form>

        <form class="remove-account" action="{{ route('profile.removeUser') }}" method="POST">
            @csrf
            <input type="hidden" name="userId" value="{{ $user->userId }}">
            <button type="submit">Remove Account</button>
        </form>
    </div>
</body>
</html>
