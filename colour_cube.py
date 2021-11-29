import pygame, sys, random, os

pygame.init()
screen = pygame.display.set_mode((800, 600))
pygame.display.set_caption("Colour Cube")
try:
    icon = pygame.image.load(os.path.join(sys._MEIPASS, "icon.png"))
except:
    icon = pygame.image.load(os.path.join(os.path.abspath("."), "icon.png"))
pygame.display.set_icon(icon)
clock = pygame.time.Clock()
font = pygame.font.SysFont("Unispace", 48)
game_over_font = pygame.font.SysFont("Unispace", 64)

TICK = 60
GRAVITY = 0.1
JUMP_STRENGTH = 3
SURROUND_WIDTH = 50
VERTICAL_START = 200
TUBE_WIDTH = 100

class Colours:
    COLOUR_1 = pygame.Color("#b14a5e")
    COLOUR_2 = pygame.Color("#76b15a")
    COLOUR_3 = pygame.Color("#73b5de")
    PLAYER_COLOURS = [COLOUR_1, COLOUR_2, COLOUR_3]
    BACKGROUND = pygame.Color("#dedacb")
    SURROUND = pygame.Color("#1e151e")
    WHITE = (255, 255, 255)
    BLACK = (0, 0, 0)

class Player:
    def __init__(self, x, y, width, height, colour, border_radius, jump_strength):
        self.position = pygame.Vector2(x, y)
        self.rect = pygame.rect.Rect(x, y, width, height)
        self.dimensions = (width, height)
        self.colour_index = 0
        self.colour = colour
        self.border_radius = border_radius
        self.velocity = pygame.Vector2()
        self.jump_strength = jump_strength

    def draw(self):
        pygame.draw.rect(screen, self.colour, self.rect, border_radius=self.border_radius)

    def update_physics(self):
        self.velocity.y += GRAVITY
        self.position.y += self.velocity.y
        self.rect = pygame.rect.Rect(self.position, self.dimensions)
        self.check_border_collision()
        self.check_tube_collision()

    def check_border_collision(self):
        if self.rect.top <= 0 + SURROUND_WIDTH:
            player_dead()
        elif self.rect.bottom >= 600 - SURROUND_WIDTH:
            player_dead()

    def check_tube_collision(self):
        for tube in tubes:
            if self.rect.colliderect(tube.top_tube) or self.rect.colliderect(tube.bottom_tube):
                player_dead()
            elif self.rect.colliderect(tube.middle_tube) and not tube.score_given:
                if self.colour == tube.middle_colour:
                    global score, gameUI
                    score += 1
                    gameUI.update_ui(score)
                    tube.score_given = True
                else:
                    player_dead()

    def jump(self):
        self.velocity.y = -self.jump_strength

    def reset(self):
        self.position.y = VERTICAL_START
        self.colour = Colours.COLOUR_1
        self.rect = pygame.rect.Rect(self.position, self.dimensions)
        self.velocity = pygame.Vector2()

    def change_colour(self):
        self.colour_index += 1
        if self.colour_index > 2:
            self.colour_index = 0
        self.colour = Colours.PLAYER_COLOURS[self.colour_index]

class Tube:
    def __init__(self, width, y_pos, gap, speed, position=800):
        self.position = position
        self.width = width
        self.gap = gap
        self.y_pos = y_pos
        self.speed = speed
        self.top_tube = pygame.rect.Rect(self.position, self.y_pos-600, self.width, 600)
        self.middle_tube = pygame.rect.Rect(self.position+self.width/3, self.y_pos, self.width/3, self.gap)
        self.bottom_tube = pygame.rect.Rect(self.position, self.y_pos+self.gap, self.width, 600)
        self.middle_colour = random.choice(Colours.PLAYER_COLOURS)
        self.score_given = False

    def update_position(self):
        self.position -= self.speed
        self.top_tube = pygame.rect.Rect(self.position, self.y_pos-600, self.width, 600)
        self.middle_tube = pygame.rect.Rect(self.position+self.width/3, self.y_pos, self.width/3, self.gap)
        self.bottom_tube = pygame.rect.Rect(self.position, self.y_pos+self.gap, self.width, 600)

    def draw(self):
        pygame.draw.rect(screen, Colours.SURROUND, self.top_tube)
        pygame.draw.rect(screen, self.middle_colour, self.middle_tube)
        pygame.draw.rect(screen, Colours.SURROUND, self.bottom_tube)

    def test_finished(self):
        if self.position <= 0-self.width:
            self.reset()

    def reset(self):
        self.position = 800
        self.gap = random.randint(100, 150)
        self.y_pos = random.randint(SURROUND_WIDTH+self.gap, 600-SURROUND_WIDTH-self.gap)
        self.middle_colour = random.choice(Colours.PLAYER_COLOURS)
        self.score_given = False

class GameUI:
    def __init__(self):
        self.width, self.height = font.size(str(0))
        self.text_surface = font.render("0", True, Colours.BACKGROUND)

    def draw(self):
        screen.blit(self.text_surface, (400-self.width/2, 9))

    def update_ui(self, score):
        self.width, self.height = font.size(str(score))
        self.text_surface = font.render(str(score), True, Colours.BACKGROUND)

class GameOverUI:
    def __init__(self):
        self.title = game_over_font.render("GAME OVER!", True, Colours.WHITE)
        self.width, self.height = game_over_font.size("GAME OVER!")
        self.title_border = pygame.rect.Rect(400-self.width/2-10, 200-10, self.width+20, self.height+20)
        self.restart_prompt = font.render("Press Space to Restart", True, Colours.WHITE)
        self.restart_width, self.restart_height = font.size("Press Space to Restart")
        self.restart_border = pygame.rect.Rect(400-self.restart_width/2-10, 400-10, self.restart_width+20, self.restart_height+20)

    def draw(self):
        pygame.draw.rect(screen, Colours.SURROUND, self.title_border)
        screen.blit(self.title, (400-self.width/2, 200))
        pygame.draw.rect(screen, Colours.SURROUND, self.restart_border)
        screen.blit(self.restart_prompt, (400-self.width/2-35, 400))

def event_test():
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        if event.type == pygame.MOUSEBUTTONDOWN and not inMenu:
            if event.button == 1:
                player.jump()
            if event.button == 3:
                player.change_colour()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE and inMenu:
                reset()

def reset():
    global inMenu, tubes, player, score, gameUI
    tubes = []
    new_tube()
    new_tube(1250)
    player.reset()
    score = 0
    gameUI.update_ui(score)
    inMenu = False

def player_dead():
    global inMenu
    inMenu = True

def new_tube(position=800):
    gap = random.randint(100, 150)
    tube = Tube(TUBE_WIDTH, random.randint(SURROUND_WIDTH+gap, 600-SURROUND_WIDTH-gap), gap, 2, position)
    tubes.append(tube)

player = Player(100, VERTICAL_START, 50, 50, Colours.COLOUR_1, 0, JUMP_STRENGTH)
tubes = []
new_tube()
new_tube(1250)
gameUI = GameUI()
gameOverUI = GameOverUI()

score = 0
running = True
inMenu = False
while running:
    clock.tick(TICK)
    event_test()
    if not inMenu:
        screen.fill(Colours.BACKGROUND)
        player.update_physics()
        player.draw()
        for tube in tubes:
            tube.update_position()
            tube.draw()
            tube.test_finished()
        pygame.draw.rect(screen, Colours.SURROUND, (0, 0, 800, SURROUND_WIDTH))
        pygame.draw.rect(screen, Colours.SURROUND, (0, 600-SURROUND_WIDTH, 800, SURROUND_WIDTH))
        gameUI.draw()
    if inMenu:
        gameOverUI.draw()
    pygame.display.update()
