#ifndef TUBE_HPP
#define TUBE_HPP

#include <SFML/Graphics.hpp>
#include "Colours.hpp"

enum TubeCollision {
    None,
    Wall,
    Colour
};

class Tube {
private:
    sf::Vector2f _position;
    sf::RectangleShape _top_rect, _bottom_rect, _middle_rect;
    int _colour_index, _gap, _middle_colour;
    bool _score_given = false;
    bool _alive = true;

public:
    Tube(float x_pos = 800);

    void update(float delta_time);

    TubeCollision test_collisions(sf::RectangleShape player_rect, int player_colour);

    void draw(sf::RenderWindow& window) const;

    inline bool get_alive() const {return _alive;}

private:
    bool rect_colliding(sf::RectangleShape rect_one, sf::RectangleShape rect_two) const;

};

#endif