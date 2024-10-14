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
// plus les cookies quand ce sera fait todo

// Récupération des valeurs envoyées via POST
$val1 = isset($_GET['val1']) ? $_GET['val1'] : 'Inconnu';
$val2 = isset($_GET['val2']) ? $_GET['val2'] : 'Inconnu';

echo "Method: " . $_SERVER['QUERY_STRING'] . "<br>";
echo "$val1  et   $val2\n"; // virer todo
// Fonction pour vérifier si la valeur est un nombre
function is_number($s) {
    return is_numeric($s);
}

// Calcul du résultat si les valeurs sont des nombres
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

echo "
<!DOCTYPE html>
<html lang=\"fr\">
    <head>
        <meta charset=\"UTF-8\">
        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
        <title>Calculator</title>
        <link rel=\"stylesheet\" href=\"../calculette.css\">
    </head>
    <body>
        <header>
            <nav>
                <ul class=\"menu\">
                    <li><a href=\"../acceuil.html\">Home</a></li>
                    <li><a href=\"../calculette.html\">Cgi Test</a></li>
                    <li><a href=\"../upload_tester.html\">Upload Test</a></li>
                    <li><a href=\"../morpion.html\">TicTacToe</a></li>
                </ul>
            </nav>
        </header>

        <main>
            <div class=\"form-section\">
                <h1>Python</h1>
                <form action=\"/cgi/cgi_script.py\" method=\"POST\">
                    <label for=\"val1\">Value 1:</label>
                    <input type=\"number\" name=\"val1\" required>
                    <label for=\"val2\">Value 2:</label>
                    <input type=\"number\" name=\"val2\" required>
                    <input type=\"submit\" value=\"Multiply\">
                    <p>res =</p>
                </form>
            </div>
            <div class=\"form-section\">
                <h1>PHP</h1>
                <form action=\"/cgi/cgi_script.php\" method=\"POST\">
                    <label for=\"val1\">Value 1:</label>
                    <input type=\"number\" name=\"val1\" required>
                    <label for=\"val2\">Value 2:</label>
                    <input type=\"number\" name=\"val2\" required>
                    <input type=\"submit\" value=\"Multiply\">
                    <p>$resultat_message</p>
                </form>
            </div>
        </main>

        <footer>
            <p>&copy; 2024 Our Site. All rights reserved.</p>
        </footer>
    </body>
</html>";
?>