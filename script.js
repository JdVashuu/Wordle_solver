//creating a board and giving each of them unique id
var board = document.getElementById("grid-container");
var count = 1;
for (var i = 0; i < 6; i++) {
    for (var j = 0; j < 5; j++) {
        var input_tile = document.createElement("input");
        input_tile.classList.add("tiless");
        input_tile.id = count.toString();
        input_tile.maxLength = 1;
        input_tile.addEventListener("input", function () {
            this.value = this.value.toUpperCase();
            this.value = this.value.toUpperCase();
            if (this.value && count < 30) {
                var nextInput = document.getElementById((count + 1).toString());
                if (nextInput) {
                    nextInput.focus();
                }
            }
        });
        board === null || board === void 0 ? void 0 : board.appendChild(input_tile);
        count++;
    }
}
