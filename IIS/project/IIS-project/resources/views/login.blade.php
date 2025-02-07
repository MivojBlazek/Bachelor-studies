<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            background: "white";
            color: black;
        }

        .content {
            background: #eee;
            padding: 2rem;
            border-radius: 10px;
            border: 1px solid #ccc;
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
            width: 100%;
            max-width: 400px;
            text-align: center;
        }

        h2 {
            margin-bottom: 1rem;
            color: #6a0dad;
        }

        form {
            display: flex;
            flex-direction: column;
            gap: 1rem;
        }

        label {
            display: block;
            text-align: left;
            font-weight: bold;
            margin-bottom: 0.5rem;
        }

        input[type="text"], input[type="password"] {
            width: 100%;
            padding: 0.5rem;
            border: 1px solid #ccc;
            border-radius: 5px;
            font-size: 1rem;
        }

        button {
            padding: 0.5rem 1rem;
            background: #6a0dad;
            color: white;
            border: none;
            border-radius: 5px;
            font-size: 1rem;
            cursor: pointer;
        }

        button:hover {
            background: #45a049;
        }

        .error-messages, .success-message {
            margin-top: 1rem;
            font-size: 0.9rem;
            text-align: left;
        }

        .error-messages ul {
            list-style: none;
            padding: 0;
            margin: 0;
            color: red;
        }

        .success-message {
            color: green;
        }

        .guest-login {
            margin-top: 1rem;
        }

        .guest-login a {
            color: #6a0dad;
            text-decoration: none;
            font-weight: bold;
        }

        .guest-login a:hover {
            text-decoration: underline;
        }

    </style>
</head>
<body>
    <div class="content">
        <h2>Login</h2>

        @if (session('success'))
            <div class="success-message">
                {{ session('success') }}
            </div>
        @endif

        <form method="POST" action="{{ route('login') }}">
            @csrf
            <div>
                <label for="username">Username:</label>
                <input type="text" id="username" name="username" required>
            </div>
            <div>
                <label for="password">Password:</label>
                <input type="password" id="password" name="password" required>
            </div>
            <button type="submit">Login</button>
            <button type="button" onclick="window.location.href='{{ route('register') }}'">Register</button>
            <div class="guest-login">
                <p>Login as a <a href="{{ route('guest.login') }}">Guest</a></p>
            </div>
        </form>

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
