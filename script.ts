//creating a board and giving each of them unique id

const board = document.getElementById("grid-container");
let count = 1;
for (let i = 0; i < 6; i++) {
    for (let j = 0; j < 5; j++) {
        const input_tile = document.createElement(`input`);
        input_tile.classList.add("tiless");
        input_tile.id = count.toString();
        input_tile.maxLength = 1;
        input_tile.addEventListener("input", function () {
            this.value = this.value.toUpperCase();
            this.value = this.value.toUpperCase();
            if (this.value && count < 30) {
                const nextInput = document.getElementById(
                    (count + 1).toString()
                );
                if (nextInput) {
                    nextInput.focus();
                }
            }
        });
        board?.appendChild(input_tile);
        count++;
    }
}
