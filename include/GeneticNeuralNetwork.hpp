#ifndef GENETIC_NEURAL_NETWORK_HPP
#define GENETIC_NEURAL_NETWORK_HPP

#include <array>
#include <cmath>

class GeneticNeuralNetwork {
private:
    static const int INPUT_NEURONS_ = 7;
    static const int LAYER_NEURONS_ = 16;
    static const int OUTPUT_NEURONS_ = 4;
    static const int MUTATION_CHANCE_ = 30;

    std::array<double, INPUT_NEURONS_> input_neurons_;
    std::array<double, LAYER_NEURONS_> layer_neurons_;
    std::array<double, OUTPUT_NEURONS_> output_neurons_;

    std::array<std::array<double, INPUT_NEURONS_>, LAYER_NEURONS_> input_to_layer_weights_;
    std::array<std::array<double, LAYER_NEURONS_>, OUTPUT_NEURONS_> layer_to_output_weights_;

public:
    GeneticNeuralNetwork(bool randomise = true);

    void randomise_weights();

    void set_input_neuron_activation(int index, double value);

    void compute_neuron_activations();

    double get_output_neuron_activation(int index);

    GeneticNeuralNetwork reproduce(GeneticNeuralNetwork partner_network) const;

    void set_input_to_layer_weight(int input_index, int layer_index, double value);

    double get_input_to_layer_weight(int input_index, int layer_index);

    void set_layer_to_output_weight(int layer_index, int output_index, double value);

    double get_layer_to_output_weight(int layer_index, int output_index);

private:
    double activation_function_(double weight_sum);

};

#endif