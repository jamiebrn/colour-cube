#ifndef TUBE_HPP
#define TUBE_HPP

#include <SFML/Graphics.hpp>

class Tube {
private:
    sf::Vector2f _position;
    sf::RectangleShape _top_rect, _bottom_rect, _middle_rect;
    int _colour_index;

public:
    Tube(sf::Vector2f position) : _position(position) {}
};

#endif