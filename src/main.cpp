#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <cmath>
#include "Tube.hpp"
#include "Player.hpp"
#include "Colours.hpp"


void set_window_icon(sf::RenderWindow& window, const char* path) {

    sf::Image icon;
    if (!icon.loadFromFile(path)) {
        std::cout << "Error: Unable to load file \"" << path << "\"\n";
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


void handle_click(sf::Event& event, Player& player, std::vector<std::unique_ptr<Tube>>& tubes, int& score) {

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
            tubes.clear();
            score = 0;
        }

    }

}


void draw_game_over(sf::RenderWindow& window, sf::Font& font, int elapsed_time) {

    sf::RectangleShape background({400, 200});
    background.setPosition({200, 200});
    background.setFillColor(Colours::surround);
    window.draw(background);

    sf::Text text;
    text.setFont(font);
    text.setFillColor(Colours::background);

    text.setCharacterSize(48);
    text.setString("GAME OVER");
    text.setOrigin({text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2});
    text.setPosition({400, 240});
    window.draw(text);

    uint8_t prompt_alpha = std::abs(std::sin(elapsed_time / 400.0)) * 255;
    text.setCharacterSize(26);
    text.setString("Left Click to Restart");
    text.setOrigin({text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2});
    text.setFillColor({Colours::background.r, Colours::background.g, Colours::background.b, prompt_alpha});
    text.setPosition({400, 345});
    window.draw(text);

}


int main() {

    srand((unsigned)time(0));

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Colour Cube");
    set_window_icon(window, "resources/icon.png");

    sf::Font font;
    if (!font.loadFromFile("resources/unispace.ttf")) {
        std::cout << "Error: Unable to load file \"resources/unispace.ttf\"\n";
        return -1;
    }
    
    sf::Clock delta_clock;
    sf::Clock running_clock;

    Player player({100, 200});
    std::vector<std::unique_ptr<Tube>> tubes;

    std::unique_ptr<Tube> tube = std::make_unique<Tube>(Tube());
    tubes.push_back(std::move(tube));
    tube.reset();
    tube = std::make_unique<Tube>(Tube(1250));
    tubes.push_back(std::move(tube));

    int score = 0;

    while (window.isOpen()) {

        float delta_time = delta_clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseButtonPressed)
                handle_click(event, player, tubes, score);

        }


        if (player.get_alive()) {

            bool got_score = player.update(delta_time, tubes);
            if (got_score)
                score++;

            for (auto& tube : tubes) {
                tube->update(delta_time);
            }

        }


        window.clear(Colours::background);
        
        draw_surround(window);

        for (auto const& tube : tubes) {
            tube->draw(window);
        }

        player.draw(window);

        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(score));
        text.setCharacterSize(32);
        text.setFillColor(Colours::background);
        text.setOrigin({text.getGlobalBounds().width / 2.0, text.getGlobalBounds().height / 2.0});
        text.setPosition({400, 35});
        window.draw(text);

        if (!player.get_alive())
            draw_game_over(window, font, running_clock.getElapsedTime().asMilliseconds());

        window.display();

    }

    return 0;
}