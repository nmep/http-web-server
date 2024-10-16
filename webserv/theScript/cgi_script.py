import cgi
import os
from datetime import datetime, timezone

print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
if 'HTTP_CONNECTION' in os.environ:
    print(f"Connection: {os.environ['CONNECTION']}")
if 'SERVER_NAME' in os.environ:
    print(f"Server: {os.environ['SERVER_NAME']}")

current_date = datetime.now(timezone.utc).strftime("%a, %d %b %Y %H:%M:%S GMT")
print(f"Date: {current_date}")
print()

form = cgi.FieldStorage()
val1 = form.getvalue("val1", "Inconnu")
val2 = form.getvalue("val2", "Inconnu")

def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        return False

if is_number(val1) and is_number(val2):
    res = float(val1) * float(val2)
    if res.is_integer():
        resultat_message = f"res = {int(res)}"
    else:
        resultat_message = f"res = {res}"
else:
    resultat_message = "Error: Please enter valid numbers."
print(f"""<!DOCTYPE html>
<html lang="fr">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Calculator</title>
        <link rel="stylesheet" href="../calculette.css">
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
                <h1>Post with Python</h1>
                <form action="/cgi/cgi_script.py" method="POST">
                    <label for="val1">Value 1:</label>
                    <input type="number" name="val1" required>
                    <label for="val2">Value 2:</label>
                    <input type="number" name="val2" required>
                    <input class="custom_color" type="submit" value="Multiply">
                    <p>{resultat_message}</p>
                </form>
            </div>
            <div class="form-section">
                <h1>Get with PHP</h1>
                <form action="/cgi/cgi_script.php" method="GET">
                    <label for="val1">Value 1:</label>
                    <input type="number" name="val1" required>
                    <label for="val2">Value 2:</label>
                    <input type="number" name="val2" required>
                    <input class="custom_color" type="submit" value="Multiply">
                    <p>res =</p>
                </form>
            </div>
        </main>
        

        <footer class="custom_color">
            <p>&copy; 2024 Our Site. All rights reserved.</p>
        </footer>
    </body>
</html>""")