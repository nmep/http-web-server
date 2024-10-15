import os
import cgi
from datetime import datetime, timedelta, timezone

print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
if 'HTTP_CONNECTION' in os.environ:
    print(f"Connection: {os.environ['CONNECTION']}")
if 'SERVER_NAME' in os.environ:
    print(f"Server: {os.environ['SERVER_NAME']}")

current_date = datetime.now(timezone.utc).strftime("%a, %d %b %Y %H:%M:%S GMT")
print(f"Date: {current_date}")

form = cgi.FieldStorage()
username = form.getvalue("username", "Inconnu")
color = form.getvalue("color", "Inconnu")

current_date = datetime.now(timezone.utc).strftime("%a, %d %b %Y %H:%M:%S GMT")
expiration_date = datetime.now(timezone.utc) + timedelta(days=30)
formatted_date = expiration_date.strftime("%a, %d-%b-%Y %H:%M:%S GMT")
print(f"Set-Cookie: username={username}; Expires={formatted_date}; Path=/")
print(f"Set-Cookie: color={color}; Expires={formatted_date}; Path=/")
print()
print(f"""<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Test site</title>
    <link rel="stylesheet" href="../personalise.css">
    <script src="script.js"></script>
</head>
<body>
    <header id="header">
        <nav>
            <ul class="menu">
                <li><a href="../acceuil.html">Home</a></li>
                <li><a href="../calculette.html">Cgi Test</a></li>
                <li><a href="../upload_tester.html">Upload Test</a></li>
                <li><a href="../personalise.html">Customize</a></li>
                <li><a href="../morpion.html">TicTacToe</a></li>
            </ul>
        </nav>
    </header>

    <main>
        <p id="prenom">no one</p>
        <form action="/cgi/cgi_set_cookies.py" method="POST">
            <label for="username"></label>
            <input type="string" name="username" required>
            <label for="color"></label>
            <input type="color" name="color" required>
            <input type="submit" value="Confirm">
        </form>
    </main>

    <footer>
        <p>&copy; 2024 Our Site. All rights reserved.</p>
    </footer>
</body>
</html>""")