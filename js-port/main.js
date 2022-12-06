
var y = 200;
var dy = 0;
var colour = 0;
var gameover = false;
var last_tick = Date.now();
var left_click = false;
var right_click = false;
var score = 0;
const tubes = [];

class Rect {
    constructor(x, y, width, height) {
        this.update(x, y, width, height);
    }

    update(x = this.x, y = this.y, width = this.width, height = this.height) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }

    isColliding(other) {
        return (this.x < other.x + other.width &&
        this.x + this.width > other.x &&
        this.y < other.y + other.height &&
        this.y + this.height > other.y)
    }
}

class Tube {
    constructor(x_pos = 800) {
        this.x_pos = x_pos;
        this.gap = Math.floor(Math.random() * 51) + 100;
        this.y_pos = Math.floor(Math.random() * (500 - this.gap * 2)) + this.gap;
        this.top_rect = new Rect(this.x_pos, this.y_pos - 600, 100, 600);
        this.middle_rect = new Rect(this.x_pos + 100/3, this.y_pos, 100/3, this.gap);
        this.bottom_rect = new Rect(this.x_pos, this.y_pos + this.gap, 100, 600);
        this.middle_colour = Math.floor(Math.random() * 3);
        this.score_given = false;
    }

    update(dt) {
        this.x_pos -= 120 * dt  / 1000;
        this.top_rect.update(this.x_pos);
        this.middle_rect.update(this.x_pos + 100/3);
        this.bottom_rect.update(this.x_pos);

        if (this.x_pos < -100) {
            this.x_pos = 800;
            this.gap = Math.floor(Math.random() * 51) + 100;
            this.y_pos = Math.floor(Math.random() * (500 - this.gap * 2)) + this.gap;
            this.top_rect.update(this.x_pos, this.y_pos - 600);
            this.middle_rect.update(this.x_pos + 100/3, this.y_pos, 100/3, this.gap);
            this.bottom_rect.update(this.x_pos, this.y_pos + this.gap);
            this.middle_colour = Math.floor(Math.random() * 3);
            this.score_given = false;
        }
    }

    testCollisions() {
        var player_rect = new Rect(100, y, 50, 50);
        if (this.top_rect.isColliding(player_rect) || this.bottom_rect.isColliding(player_rect)) {
            return 1;
        }
        else if (this.middle_rect.isColliding(player_rect)) {
            if (colour == this.middle_colour && !this.score_given) {
                this.score_given = true;
                return 2;
            }
            else if (colour != this.middle_colour) {
                return 1;
            }
        }
        return 0;
    }

    draw(ctx) {
        ctx.fillStyle = "#1e151e";
        ctx.fillRect(this.top_rect.x, this.top_rect.y, this.top_rect.width, this.top_rect.height);
        ctx.fillRect(this.bottom_rect.x, this.bottom_rect.y, this.bottom_rect.width, this.bottom_rect.height);

        ctx.fillStyle = playerNumToColour(this.middle_colour);
        ctx.fillRect(this.middle_rect.x, this.middle_rect.y, this.middle_rect.width, this.middle_rect.height);
    }
}

function init() {
    const canvas = document.getElementById("gameCanvas");
    const ctx = canvas.getContext("2d");
    canvas.width = 800;
    canvas.height = 600;

    addEventListener("mousedown", function(e){
        if (e.button === 0) {
            left_click = true;
        } else if (e.button === 2) {
            right_click = true;
        }
    });

    tubes.push(new Tube());
    tubes.push(new Tube(1250));

    masterCall(ctx);
}

function update() {
    
    var date_now = Date.now();
    var dt = date_now - last_tick;
    last_tick = date_now;

    if (!gameover) {
        gameloop(dt);
    }
    else {
        if (left_click) {
            left_click = false;
            y = 200;
            dy = 0;
            colour = 0;
            gameover = false;
            score = 0;
            tubes.length = 0;
            tubes.push(new Tube());
            tubes.push(new Tube(1250));
        }
    }

}

function gameloop(dt) {

    // Player gravity
    y += dy * 10 * dt / 1000;
    dy += 25 * dt / 1000;

    if (y <= 50 || y >= 500) {
        gameover = true;
    }

    if (left_click) {
        dy = -15;
        left_click = false;
    }

    if (right_click) {
        if (++colour > 2) {
            colour = 0;
        }
        right_click = false;
    }

    for (let i = 0; i < tubes.length; i++) {
        tubes[i].update(dt);
        collided = tubes[i].testCollisions();
        if (collided == 1) {
            gameover = true;
        }
        if (collided == 2) {
            score++;
        }
    }
}

function draw(ctx) {

    // Background clear
    ctx.fillStyle = "#dedacb";
    ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);

    for (let i = 0; i < tubes.length; i++) {
        tubes[i].draw(ctx);
    }

    // Player draw
    ctx.fillStyle = playerNumToColour(colour);
    ctx.fillRect(100, y, 50, 50);

    // Borders
    ctx.fillStyle = "#1e151e";
    ctx.fillRect(0, 0, 800, 50);
    ctx.fillRect(0, 550, 800, 50);

    // Draw score
    ctx.fillStyle = "#dedacb";
    ctx.font = "32px Unispace";
    ctx.textAlign = "center";
    ctx.fillText(score.toString(), 400, 35);

    // Gameover UI
    if (gameover) {
        ctx.fillStyle = "#1e151e";
        ctx.fillRect(200, 200, 400, 200);

        ctx.fillStyle = "#dedacb";
        ctx.font = "48px Unispace";
        ctx.textAlign = "center";
        ctx.fillText("GAME OVER", 400, 270);

        ctx.fillStyle = "rgba(222, 218, 203, " + Math.abs(Math.sin(Date.now() / 400)) + ")";
        ctx.font = "26px Unispace";
        ctx.fillText("Left Click to Restart", 400, 360);
    }
}

function playerNumToColour(num) {
    if (num == 0) {
        return "#b14a5e";
    }
    else if (num == 1) {
        return "#76b15a";
    }
    else {
        return "#73b5de";
    }
}

function masterCall(ctx) {
    function callFunctions() {
        update();
        draw(ctx);
        requestAnimationFrame(callFunctions);
    };
    requestAnimationFrame(callFunctions);
}

window.addEventListener("load", init);
