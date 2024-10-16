import os
import cgi
from datetime import datetime, timedelta, timezone

print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
if 'HTTP_CONNECTION' in os.environ:
    print(f"Connection: {os.environ['CONNECTION']}")
if 'SERVER_NAME' in os.environ:
    print(f"Server: {os.environ['SERVER_NAME']}")

current_date = (datetime.now(timezone.utc) + timedelta(hours=2)).strftime("%a, %d %b %Y %H:%M:%S GMT")
print(f"Date: {current_date}")

form = cgi.FieldStorage()
username = form.getvalue("username", "Unknown")
color = form.getvalue("color", "#ffffff")

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
    <script src="../script.js"></script>
</head>
<body>
    <header class="custom_color">
        <div class="user-info">
            <img src="../images.png" alt="Logo de l'ecole 42" class="user-logo">
            <p id="username-display"></p>
        </div>
        <nav>
            <ul class="menu">
                <li><a href="../acceuil.html">Home</a></li>
                <li><a href="../calculette.html">Cgi Test</a></li>
                <li><a href="../upload_tester.html">Upload Test</a></li>
                <li><a href="../delete.html">Delete Test</a></li>
                <li><a href="../personalise.html">Customize</a></li>
            </ul>
        </nav>
    </header>

    <main>
        <div class="form-section">
            <h1>Customize your page and choose a username</h1>
            <form action="/cgi/cgi_set_cookies.py" method="POST" onsubmit="before_actu()">
                <label for="username">Username:</label>
                <input type="string" name="username" required maxlength="15">
                <label for="color">Color:</label>
                <input id="color_button" type="color" name="color" required>
                <input class="custom_color" type="submit" value="Confirm">
            </form>
        </div>
    </main>

    <footer class="custom_color">
        <p>&copy; 2024 Our Site. All rights reserved.</p>
    </footer>
</body>
</html>""")