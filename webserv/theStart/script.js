function getCookie(name) {
    let value = `; ${document.cookie}`;
    let parts = value.split(`; ${name}=`);
    if (parts.length === 2) return parts.pop().split(';').shift();
}

function appliquerCouleursDepuisCookies() {
    const couleur = getCookie('color');
    if (couleur) {
        let head = document.getElementById('header');
        head.style.backgroundColor = couleur;


    }
}

window.onload = function() {
    appliquerCouleursDepuisCookies();
};
