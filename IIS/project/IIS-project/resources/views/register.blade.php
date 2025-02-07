<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Register</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            background: white;
            color: #000;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
        }

        .content {
            width: 100%;
            max-width: 400px;
            padding: 2rem;
            border: 1px solid #ccc;
            border-radius: 10px;
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
            background: #eee;
            text-align: center;
        }
        h2 {
            color: #6a0dad;
            margin-bottom: 1.5rem;
        }
        form {
            display: flex;
            flex-direction: column;
            gap: 1rem;
        }
        label {
            font-weight: bold;
            text-align: left;
        }
        input[type="text"],
        input[type="email"],
        input[type="number"],
        input[type="password"] {
            width: 100%;
            padding: 0.5rem;
            border: 1px solid #ccc;
            border-radius: 5px;
            font-size: 1rem;
        }
        button {
            padding: 0.7rem;
            border: none;
            border-radius: 5px;
            font-size: 1rem;
            cursor: pointer;
        }
        button[type="submit"] {
            background: #6a0dad;
            color: white;
        }
        button[type="submit"]:hover {
            background: #5a0c9e;
        }
        button[type="button"] {
            background: #ccc;
            color: #333;
        }
        button[type="button"]:hover {
            background: #bbb;
        }
        p {
            margin-top: 1rem;
            font-size: 0.9rem;
        }
        p strong {
            color: #6a0dad;
        }
        .error-messages {
            margin-top: 1rem;
            color: red;
            font-size: 0.9rem;
            text-align: left;
        }
        .error-messages ul {
            padding: 0;
            margin: 0;
            list-style: none;
        }
    </style>
</head>
<body>
    <div class="content">
        <h2>Register</h2>
        <form method="POST" action="{{ route('register') }}">
            @csrf
            <div>
                <label for="username">*Username:</label>
                <input type="text" id="username" name="username" value="{{ old('username') }}" required>
            </div>
            <div>
                <label for="name">*Name:</label>
                <input type="text" id="name" name="name" value="{{ old('name') }}" required>
            </div>
            <div>
                <label for="email">*Email:</label>
                <input type="email" id="email" name="email" value="{{ old('email') }}" required>
            </div>
            <div>
                <label for="age">Age:</label>
                <input type="number" id="age" name="age" min="1" max="150" value="{{ old('age') }}">
            </div>
            <div>
                <label for="password">*Password:</label>
                <input type="password" id="password" name="password" required>
            </div>
            <div>
                <label for="password_confirmation">*Confirm password:</label>
                <input type="password" id="password_confirmation" name="password_confirmation" required>
            </div>
            <button type="submit">Register</button>
            <button type="button" onclick="window.location.href='{{ route('login') }}'">Back to login</button>
        </form>
        <p>Fields marked with <strong>*</strong> are required</p>

        @if ($errors->any())
            <div class="error-messages">
                <ul>
                    @foreach ($errors->all() as $error)
                        <li>{{ $error }}</li>
                    @endforeach
                </ul>
            </div>
        @endif
    </div>
</body>
</html>
