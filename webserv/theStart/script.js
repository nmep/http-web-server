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

function change_username() {
    const username = getCookie('username');
    if (username) {
        let element = document.getElementById("username-display");
        element.textContent = username;
    }
}

window.onload = function() {
    change_color();
    change_username();
};

// function before_actu(){
//     let element = document.getElementById("color_button");
//     const couleur = element.value;
//     if (couleur) {
//         let elements = document.getElementsByClassName("custom_color");
//         for (let i = 0; i < elements.length; i++) {
//             elements[i].style.backgroundColor = couleur;
//         }
//     }
// }

function deleteRessource() {
    const ressource = document.getElementById('ressource').value;

    fetch('./cgi/cgi_delete_ressource.py?ressource=' + encodeURIComponent(ressource), { method: 'DELETE', })
    .then(response => {
            return response.text();
    })
    .then(data => {
        document.getElementById('responseMessage').innerText = data;
    })
}

