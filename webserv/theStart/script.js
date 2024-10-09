var on = true

function changerCouleur() {
    let texte = document.getElementById("texte");

    if (on == true)
    {
        texte.style.color = "blue";
        on = false;
    }
    else
    {
        texte.style.color = "red";
        on = true;
    }
}