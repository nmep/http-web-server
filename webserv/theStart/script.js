function getCookie(name) {
    let value = `; ${document.cookie}`;
    let parts = value.split(`; ${name}=`);
    if (parts.length === 2) return parts.pop().split(';').shift();
}

function change_color() {
    const couleur = getCookie('color');
    if (couleur) {
        let elements = document.getElementsByClassName("custom_color");
        for (let i = 0; i < elements.length; i++) {
            elements[i].style.backgroundColor = couleur;
        }
    }
}

window.onload = function() {
    change_color();
};

function before_actu(){
    let element = document.getElementById("color_button");
    const couleur = element.value;
    if (couleur) {
        let elements = document.getElementsByClassName("custom_color");
        for (let i = 0; i < elements.length; i++) {
            elements[i].style.backgroundColor = couleur;
        }
    }
}

