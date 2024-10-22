#ifndef COLOURS_HPP
#define COLOURS_HPP

#include <SFML/Graphics.hpp>
#include <array>

namespace Colours {

const std::array<sf::Color, 3> list = {sf::Color(177, 74, 94), sf::Color(118, 177, 90), sf::Color(115, 181, 222)};
const sf::Color background = {222, 218, 203};
const sf::Color surround = {30, 21, 30};

};

#endif