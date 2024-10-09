#!/usr/bin/env python

import cgi

# Pour le rendu HTML
print("Content-Type: text/html")
print()

# Traitement des données envoyées
form = cgi.FieldStorage()
data = form.getvalue("data", "Aucune donnée reçue")

# Réponse HTML
print(f"<html><body><h1>Données reçues : {data}</h1></body></html>")