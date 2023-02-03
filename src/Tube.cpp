#include "Tube.hpp"

Tube::Tube(float x_pos = 800) {

    _position = {x_pos, 0};
    

}

void update(float delta_time);

bool test_collisions(sf::RectangleShape player_rect);

void draw(sf::RenderWindow& window);