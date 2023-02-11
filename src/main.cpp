#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <cmath>
#include <algorithm>

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
    surround.setSize({800, 200});
    window.draw(surround);

}


int main() {

    srand((unsigned)time(0));

    sf::RenderWindow window(sf::VideoMode({800, 720}), "Colour Cube");
    set_window_icon(window, "resources/icon.png");

    sf::Font font;
    if (!font.loadFromFile("resources/unispace.ttf")) {
        std::cout << "Error: Unable to load file \"resources/unispace.ttf\"\n";
        return -1;
    }
    
    sf::Clock delta_clock;
    sf::Clock running_clock;

    std::vector<std::unique_ptr<Tube>> tubes;

    std::unique_ptr<Tube> tube = std::make_unique<Tube>(Tube());
    tubes.push_back(std::move(tube));
    tube.reset();
    tube = std::make_unique<Tube>(Tube(1250));
    tubes.push_back(std::move(tube));

    int generation = 0;
    float generation_time = 0;
    float longest_generation = 0;

    std::vector<Player> ai_players;
    for (int i = 0; i < 100; i++) {
        ai_players.push_back(Player({100, 300}));
    }

    while (window.isOpen()) {

        float delta_time = delta_clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

        }

        generation_time += delta_time;

        int players_alive = 0;
        for (Player& player : ai_players) {
            if (player.get_alive()) {
                players_alive++;
                player.update(delta_time, tubes);
            }
        }

        if (players_alive == 0) {

            // Put ai which lasted longest at beginning of array
            std::sort(ai_players.begin(), ai_players.end(), 
                [](Player const& a, Player const& b) -> bool
            { 
                return a.get_time_alive() > b.get_time_alive(); 
            });

            longest_generation = std::max(longest_generation, ai_players.at(0).get_time_alive());

            // Get the 10 networks that lasted the longest
            std::vector<GeneticNeuralNetwork> best_networks;
            for (int i = 0; i < 10; i++) {
                best_networks.push_back(ai_players.at(i).get_neural_network());
            }

            // "Breed" new networks for each player object, based on best networks
            // Assign them to player objects
            for (int i = 0; i < 100; i++) {
                GeneticNeuralNetwork const& parent_one = best_networks.at(rand() % 10);
                GeneticNeuralNetwork const& parent_two = best_networks.at(rand() % 10);
                GeneticNeuralNetwork new_network = parent_one.reproduce(parent_two);

                ai_players.at(i).set_neural_network(new_network);
                ai_players.at(i).reset();
            }

            // Reset tubes
            tubes.clear();
            std::unique_ptr<Tube> tube = std::make_unique<Tube>(Tube());
            tubes.push_back(std::move(tube));
            tube.reset();
            tube = std::make_unique<Tube>(Tube(1250));
            tubes.push_back(std::move(tube));

            generation++;
            generation_time = 0;

        }

        for (auto& tube : tubes) {
            tube->update(delta_time);
        }

        for (int i = 0; i < tubes.size(); i++) {
            if (!tubes[i]->get_alive()) {
                tubes.erase(tubes.begin() + i--);
                std::unique_ptr<Tube> tube = std::make_unique<Tube>(Tube());
                tubes.push_back(std::move(tube));
            }
        }

        window.clear(Colours::background);
        
        draw_surround(window);

        for (auto const& tube : tubes) {
            tube->draw(window);
        }

        for (Player& player : ai_players) {
            if (player.get_alive())
                player.draw(window);
        }

        sf::Text text;
        text.setFont(font);
        text.setPosition({20, 610 - 30});
        text.setString("Generation: " + std::to_string(generation));
        window.draw(text);

        text.setPosition({20, 650 - 30});
        text.setString("Time: " + std::to_string(generation_time));
        window.draw(text);

        text.setPosition({20, 690 - 30});
        text.setString("Longest time: " + std::to_string(longest_generation));
        window.draw(text);

        text.setPosition({600, 660});
        text.setString(std::to_string(players_alive) + " left");
        window.draw(text);

        window.display();

    }

    return 0;
}