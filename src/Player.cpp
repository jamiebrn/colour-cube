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

    // Neural network

    time_alive_ += delta_time;

    // AI "Player" values
    neural_network_.set_input_neuron_activation(0, _position.y);
    neural_network_.set_input_neuron_activation(1, _velocity.y);
    neural_network_.set_input_neuron_activation(2, _colour_index);

    // Environment values
    neural_network_.set_input_neuron_activation(3, tubes.at(0)->get_x_pos() - _position.x + 50);
    neural_network_.set_input_neuron_activation(4, tubes.at(0)->get_y_pos() - _position.y + 50);
    neural_network_.set_input_neuron_activation(5, tubes.at(0)->get_gap());
    neural_network_.set_input_neuron_activation(6, tubes.at(0)->get_colour_index());

    // Recompute neural network with newly provided information
    neural_network_.compute_neuron_activations();

    // Act on "stimuli" data given, after processing it
    if (neural_network_.get_output_neuron_activation(0) > 0.7)
        jump();
    
    if (neural_network_.get_output_neuron_activation(1) > 0.5)
        _colour_index = 0;
    
    if (neural_network_.get_output_neuron_activation(2) > 0.5)
        _colour_index = 1;
    
    if (neural_network_.get_output_neuron_activation(3) > 0.5)
        _colour_index = 2;

    return scored;

}

void Player::reset() {

    _position = _start_position;
    _velocity.y = 0;
    _colour_index = 0;
    time_alive_ = 0;
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

void Player::set_neural_network(GeneticNeuralNetwork neural_network) {

    neural_network_ = neural_network;

}

GeneticNeuralNetwork Player::get_neural_network() {

    return neural_network_;

}