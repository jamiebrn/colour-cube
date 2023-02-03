#ifndef TUBE_HPP
#define TUBE_HPP

#include <SFML/Graphics.hpp>

class Tube {
private:
    sf::Vector2f _position;
    sf::RectangleShape _top_rect, _bottom_rect, _middle_rect;
    int _colour_index;

public:
    Tube(float x_pos = 800) : _position({x_pos, 0}) {}

    void update(float delta_time);

    bool test_collisions(sf::RectangleShape player_rect);

    void draw(sf::RenderWindow& window);

};

#endif