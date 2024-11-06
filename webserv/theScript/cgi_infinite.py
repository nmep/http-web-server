import cgi
import os
from datetime import datetime, timedelta, timezone
import time

print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
if 'HTTP_CONNECTION' in os.environ:
    print(f"Connection: {os.environ['CONNECTION']}")
if 'SERVER_NAME' in os.environ:
    print(f"Server: {os.environ['SERVER_NAME']}")

current_date = (datetime.now(timezone.utc) + timedelta(hours=2)).strftime("%a, %d %b %Y %H:%M:%S GMT")
print(f"Date: {current_date}")
print()

while True:
    time.sleep(0.0001) # 100 microsecondes pour pas spam
