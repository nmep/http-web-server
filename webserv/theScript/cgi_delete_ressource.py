import cgi
import os
import urllib.parse
from datetime import datetime, timedelta, timezone

print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
if 'HTTP_CONNECTION' in os.environ:
    print(f"Connection: {os.environ['CONNECTION']}")
if 'SERVER_NAME' in os.environ:
    print(f"Server: {os.environ['SERVER_NAME']}")
current_date = (datetime.now(timezone.utc) + timedelta(hours=2)).strftime("%a, %d %b %Y %H:%M:%S GMT")
print(f"Date: {current_date}")
print()

if os.environ['REQUEST_METHOD'] == 'DELETE':
    query_string = os.environ.get('QUERY_STRING', '')
    params = urllib.parse.parse_qs(query_string)
    ressource = params.get('ressource', [''])[0]

    if ressource and os.path.exists(ressource):
        os.remove(ressource)
        print("The resource has been deleted successfully")
    else:
        print("The resource doesn't exist")
else:
    print("<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Test site</title>\n<link rel=\"stylesheet\" href=\"../delete.css\">\n<script src=\"../script.js\"></script>\n</head>\n<body>\n<header class=\"custom_color\">\n<div class=\"user-info\">\n<img src=\"../images.png\" alt=\"Logo de l'ecole 42\" class=\"user-logo\">\n<p id=\"username-display\"></p>\n</div>\n<nav>\n<ul class=\"menu\">\n<li><a href=\"../acceuil.html\">Home</a></li>\n<li><a href=\"../calculette.html\">Cgi Test</a></li>\n<li><a href=\"../upload_tester.html\">Upload Test</a></li>\n<li><a href=\"../delete.html\">Delete Test</a></li>\n<li><a href=\"../personalise.html\">Customize</a></li>\n</ul>\n</nav>\n</header>\n<main>\n<h1>Delete a Resource</h1>\n<form id=\"deleteForm\">\n<label for=\"ressource\">Select a resource to delete:</label>\n<select id=\"ressource\" name=\"ressource\">\n<option value=\"./theStart/test_directory/page1.txt\">test_directory/page1.txt</option>\n<option value=\"./theStart/test_directory/page2.html\">test_directory/page2.html</option>\n<option value=\"./theStart/test_directory/empty_page.txt\">test_directory/empty_page.txt</option>\n</select>\n<button type=\"button\" class=\"button\" onclick=\"deleteRessource()\">Delete</button>\n</form>\n<p id=\"responseMessage\">You need to use DELETE for this script</p>\n</main>\n<footer class=\"custom_color\">\n<p>&copy; 2024 Our Site. All rights reserved.</p>\n</footer>\n</body>\n</html>")