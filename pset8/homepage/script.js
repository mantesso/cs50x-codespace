document.addEventListener("DOMContentLoaded", (event) => {
    bgBtn = document.getElementById("randomBg");
    bgBtn.addEventListener('click', changeBgColor);
});

function changeBgColor() {
    // random RGB values in the higher range to ensure contrats with black text
    let random_r = Math.floor(Math.random() * 100) + 156;
    let random_g = Math.floor(Math.random() * 100) + 156;
    let random_b = Math.floor(Math.random() * 100) + 156;

    document.body.style.background = `rgb(${random_r},${random_g},${random_b})`;
}

