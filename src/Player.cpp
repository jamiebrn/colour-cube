#include "Player.hpp"

bool Player::update(float delta_time, std::vector<std::unique_ptr<Tube>> const& tubes) {

    bool scored = false;

    _position.y += _velocity.y * 10 * delta_time;
    _velocity.y += 25 * delta_time;

    if (_position.y <= 50 || _position.y >= 500)
        _alive = false;

    sf::RectangleShape rect({50, 50});
    rect.setPosition(_position);
    for (auto const& tube : tubes) {

        TubeCollision tube_collision = tube->test_collisions(rect, _colour_index);
        if (tube_collision == TubeCollision::Wall) {
            _alive = false;
        }
        else if (tube_collision == TubeCollision::Colour) {
            scored = true;
        }

    }

    return scored;

}

void Player::reset() {

    _position = _start_position;
    _velocity.y = 0;
    _alive = true;

}

void Player::jump() {

    _velocity.y = -15;
    
}
    
void Player::cycle_colour() {

    _colour_index++;
    _colour_index %= Colours::list.size();

}

void Player::draw(sf::RenderWindow& window) const {

    sf::RectangleShape rect({50, 50});
    rect.setPosition(_position);
    rect.setFillColor(Colours::list[_colour_index]);

    window.draw(rect);

}