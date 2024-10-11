<?php
header("Content-Type: text/html; charset=UTF-8");

$val1 = isset($_GET['val1']) ? $_GET['val1'] : 'Inconnu';
$val2 = isset($_GET['val2']) ? $_GET['val2'] : 'Inconnu';

function is_number($s) {
    return is_numeric($s);
}

if (is_number($val1) && is_number($val2)) {
    $res = floatval($val1) * floatval($val2);
    if (intval($res) == $res) {
        $resultat_message = "Résultat : " . intval($res);
    } else {
        $resultat_message = "Résultat : " . $res;
    }
} else {
    $resultat_message = "Erreur : Veuillez entrer des nombres valides.";
}

echo "
<html>
    <head>
        <meta charset=\"UTF-8\">
        <title>Réponse du CGI</title>
    </head>
    <body>
        <h1>Données reçues</h1>
        <p>$resultat_message</p>
    </body>
</html>
";
?>