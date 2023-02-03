#include "Player.hpp"

void Player::update(float delta_time, std::vector<std::unique_ptr<Tube>>& tubes) {

    _position.y += _velocity.y * 10 * delta_time;
    _velocity.y += 25 * delta_time;

    if (_position.y <= 50 || _position.y >= 500)
        _alive = false;

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

void Player::draw(sf::RenderWindow& window) {

    sf::RectangleShape rect({50, 50});
    rect.setPosition(_position);
    rect.setFillColor(Colours::list[_colour_index]);

    window.draw(rect);

}