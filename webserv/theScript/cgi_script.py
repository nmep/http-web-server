import cgi
import cgitb

# Activer le débogage CGI
cgitb.enable()
# Pour le rendu HTML
print("Content-Type: text/html")
print()  # Cela envoie une ligne vide qui sépare les en-têtes du corps

# Traitement des données envoyées
form = cgi.FieldStorage()
age = form.getvalue("age", "Inconnu")    # Valeur par défaut si 'age' n'est pas fourni

# Réponse HTML
print(f"""
<html>
    <head>
        <title>Réponse du CGI</title>
    </head>
    <body>
        <h1>Données reçues</h1>
        <p>Âge : {age}</p>
    </body>
</html>
""")