#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Tube.hpp"
#include "Colours.hpp"

class Player {
private:
    sf::Vector2f _position, _start_position, _velocity;
    int _colour_index = 0;
    bool _alive = true;

public:
    Player(sf::Vector2f position) : _position(position), _start_position(position) {}

    bool update(float delta_time, std::vector<std::unique_ptr<Tube>> const& tubes);

    void draw(sf::RenderWindow& window) const;

    void jump();
    
    void cycle_colour();

    inline bool get_alive() const {return _alive;}

    void reset();

};

#endif