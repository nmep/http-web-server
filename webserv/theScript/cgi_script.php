<?php
echo "HTTP/1.1 200 OK\n";
echo "Content-Type: text/html\n";
if (isset($_SERVER['CONNECTION'])) {
    echo "Connection: " . $_SERVER['CONNECTION'] . "\r\n";
}
if (isset($_SERVER['SERVER_NAME'])) {
    echo "Server: " . $_SERVER['SERVER_NAME'] . "\r\n";
}
if (isset($_SERVER['DATE'])) {
    echo "Date: " . $_SERVER['DATE'] . "\r\n";
}
echo "\r\n";

$query_string = $_SERVER['QUERY_STRING'];
parse_str($query_string, $output);

$val1 = isset($output['val1']) ? $output['val1'] : 'Inconnu';
$val2 = isset($output['val2']) ? $output['val2'] : 'Inconnu';

function is_number($s) {
    return is_numeric($s);
}

if (is_number($val1) && is_number($val2)) {
    $res = floatval($val1) * floatval($val2);
    if (intval($res) == $res) {
        $resultat_message = "res = " . intval($res);
    } else {
        $resultat_message = "res = " . $res;
    }
} else {
    $resultat_message = "Error: Please enter valid numbers.";
}

echo "<!DOCTYPE html>
<html lang=\"fr\">
    <head>
        <meta charset=\"UTF-8\">
        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
        <title>Calculator</title>
        <link rel=\"stylesheet\" href=\"../calculette.css\">
        <script src=\"../script.js\"></script>
    </head>
    <body>
        <header class=\"custom_color\">
            <div class=\"user-info\">
                <img src=\"../images.png\" alt=\"Logo de l'ecole 42\" class=\"user-logo\">
                <p id=\"username-display\"></p>
            </div>
            <nav>
                <ul class=\"menu\">
                    <li><a href=\"../acceuil.html\">Home</a></li>
                    <li><a href=\"../calculette.html\">Cgi Test</a></li>
                    <li><a href=\"../upload_tester.html\">Upload Test</a></li>
                    <li><a href=\"../delete.html\">Delete Test</a></li>
                    <li><a href=\"../personalise.html\">Customize</a></li>
                </ul>
            </nav>
        </header>

        <main>
            <div class=\"form-section\">
                <h1>Post with Python</h1>
                <form action=\"/cgi/cgi_script.py\" method=\"POST\">
                    <label for=\"val1\">Value 1:</label>
                    <input type=\"number\" name=\"val1\" required>
                    <label for=\"val2\">Value 2:</label>
                    <input type=\"number\" name=\"val2\" required>
                    <input class=\"custom_color\" type=\"submit\" value=\"Multiply\">
                    <p>res =</p>
                </form>
            </div>
            <div class=\"form-section\">
                <h1>Get with PHP</h1>
                <form action=\"/cgi/cgi_script.php\" method=\"GET\">
                    <label for=\"val1\">Value 1:</label>
                    <input type=\"number\" name=\"val1\" required>
                    <label for=\"val2\">Value 2:</label>
                    <input type=\"number\" name=\"val2\" required>
                    <input class=\"custom_color\" type=\"submit\" value=\"Multiply\">
                    <p>$resultat_message</p>
                </form>
            </div>
        </main>
        

        <footer class=\"custom_color\">
            <p>&copy; 2024 Our Site. All rights reserved.</p>
        </footer>
    </body>
</html>";
?>