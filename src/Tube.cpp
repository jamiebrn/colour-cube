#include "Tube.hpp"

Tube::Tube(float x_pos) {

    _position.x = x_pos;
    _gap = 100 + (rand() % 50);
    _position.y = 50 + _gap + (rand() % (500 - _gap * 2));

    _middle_colour = rand() % 3;

    _top_rect.setSize({100, 600});
    _top_rect.setFillColor(Colours::surround);
    _middle_rect.setSize({100 / 3, static_cast<float>(_gap)});
    _middle_rect.setFillColor(Colours::list[_middle_colour]);
    _bottom_rect.setSize({100, 600});
    _bottom_rect.setFillColor(Colours::surround);

}

void Tube::update(float delta_time) {

    _position.x -= 120 * delta_time;
    _top_rect.setPosition({_position.x, _position.y - 600});
    _middle_rect.setPosition({_position.x + 100 / 3, _position.y});
    _bottom_rect.setPosition({_position.x, _position.y + _gap});

    if (_position.x < -100)
        _alive = false;

}

TubeCollision Tube::test_collisions(sf::RectangleShape player_rect, int player_colour) {

    if (rect_colliding(_top_rect, player_rect) || rect_colliding(_bottom_rect, player_rect))
        return TubeCollision::Wall;
    
    if (rect_colliding(_middle_rect, player_rect)) {

        if (player_colour == _middle_colour && !_score_given) {
            _score_given = true;
            return TubeCollision::Colour;
        }
        else if (player_colour != _middle_colour) {
            return TubeCollision::Wall;
        }

    }

    return TubeCollision::None;

}

bool Tube::rect_colliding(sf::RectangleShape rect_one, sf::RectangleShape rect_two) const {

    sf::Vector2f one = rect_one.getPosition();
    sf::Vector2f one_size = rect_one.getSize();
    sf::Vector2f two = rect_two.getPosition();
    sf::Vector2f two_size = rect_two.getSize();

    return (one.x < two.x + two_size.x &&
        one.x + one_size.x > two.x &&
        one.y < two.y + two_size.y &&
        one.y + one_size.y > two.y);

}

void Tube::draw(sf::RenderWindow& window) const {

    window.draw(_top_rect);
    window.draw(_middle_rect);
    window.draw(_bottom_rect);

}




