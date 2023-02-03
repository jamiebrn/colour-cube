#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "Tube.hpp"
#include "Player.hpp"
#include "Colours.hpp"


void set_icon(sf::RenderWindow& window, const char* path) {

    sf::Image icon;
    if (!icon.loadFromFile("icon.png")) {
        std::cout << "Error: Unable to load file \"icon.png\"\n";
    }

    window.setIcon({icon.getSize().x, icon.getSize().y}, icon.getPixelsPtr());

}


void draw_surround(sf::RenderWindow& window) {

    sf::RectangleShape surround({800, 50});
    surround.setFillColor(Colours::surround);
    window.draw(surround);

    surround.setPosition({0, 550});
    window.draw(surround);

}


void handle_click(sf::Event& event, Player& player) {

    if (player.get_alive()) {

        if (event.mouseButton.button == sf::Mouse::Right) {
            player.cycle_colour();
        }

        if (event.mouseButton.button == sf::Mouse::Left) {
            player.jump();
        }

    }
    else {

        if (event.mouseButton.button == sf::Mouse::Left) {
            player.reset();
        }

    }

}


int main() {

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Colour Cube");
    set_icon(window, "icon.png");

    Player player({100, 200});
    std::vector<std::unique_ptr<Tube>> tubes;

    sf::Clock clock;

    while (window.isOpen()) {

        sf::Time time = clock.restart();
        float delta_time = time.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseButtonPressed)
                handle_click(event, player);

        }


        if (player.get_alive()) {
            player.update(delta_time, tubes);
        }


        window.clear(Colours::background);
        
        draw_surround(window);

        player.draw(window);


        window.display();

    }

    return 0;
}