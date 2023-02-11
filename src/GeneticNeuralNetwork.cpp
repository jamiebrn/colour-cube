#include "GeneticNeuralNetwork.hpp"

GeneticNeuralNetwork::GeneticNeuralNetwork(bool randomise) {

    if (randomise)
        randomise_weights();

}

void GeneticNeuralNetwork::randomise_weights() {

    // Randomise input to layer weights
    for (int i = 0; i < input_to_layer_weights_.size(); i++) {

        auto connection_weights = input_to_layer_weights_.at(i);

        for (int j = 0; j < connection_weights.size(); j++) {

            double random_weight = (rand() % 101) / 100;
            set_input_to_layer_weight(j, i, random_weight);

        }

    }

    // Randomise layer to output weights
    for (int i = 0; i < layer_to_output_weights_.size(); i++) {

        auto connection_weights = layer_to_output_weights_.at(i);

        for (int j = 0; j < connection_weights.size(); j++) {

            double random_weight = (rand() % 101) / 100;
            set_layer_to_output_weight(j, i, random_weight);

        }

    }

}

void GeneticNeuralNetwork::set_input_neuron_activation(int index, double value) {

    if (index >= input_neurons_.size())
        return;
    
    input_neurons_.at(index) = value;

}

void GeneticNeuralNetwork::compute_neuron_activations() {

    // Compute layer neuron values
    for (int i = 0; i < input_to_layer_weights_.size(); i++) {

        auto connection_weights = input_to_layer_weights_.at(i);
        double weight_sum = 0;

        for (int j = 0; j < connection_weights.size(); j++) {

            weight_sum += input_neurons_.at(j) * connection_weights.at(j);

        }

        layer_neurons_.at(i) = activation_function_(weight_sum);

    }

    // Compute output neuron values
    for (int i = 0; i < layer_to_output_weights_.size(); i++) {

        auto connection_weights = layer_to_output_weights_.at(i);
        double weight_sum = 0;

        for (int j = 0; j < connection_weights.size(); j++) {

            weight_sum += layer_neurons_.at(j) * connection_weights.at(j);

        }

        output_neurons_.at(i) = activation_function_(weight_sum);

    }

}

double GeneticNeuralNetwork::get_output_neuron_activation(int index) {

    if (index >= output_neurons_.size())
        return 0;
    
    return output_neurons_.at(index);

}

GeneticNeuralNetwork GeneticNeuralNetwork::reproduce(GeneticNeuralNetwork partner_network) const {

    // Genetics

    GeneticNeuralNetwork offspring_network(false);

    // Randomly assign either this network's weight or partner's weight
    // to new neural network's weight
    for (int i = 0; i < input_to_layer_weights_.size(); i++) {

        auto connection_weights = input_to_layer_weights_.at(i);

        for (int j = 0; j < connection_weights.size(); j++) {

            int random_value = rand() % (101 + MUTATION_CHANCE_);
            
            double this_weight = connection_weights.at(j);
            double parter_weight = partner_network.get_input_to_layer_weight(j, i);
            double new_weight = random_value > 50 ? this_weight : parter_weight;
            
            if (random_value <= MUTATION_CHANCE_) {

                int multiplier = rand() % 101;
                double amount = (rand() % 100) / 100;

                if (multiplier > 50)
                    new_weight += amount;
                else
                    new_weight -= amount;

            }

            offspring_network.set_input_to_layer_weight(j, i, new_weight);

        }

    }

    // Randomly assign either this network's weight or partner's weight
    // to new neural network's weight
    for (int i = 0; i < layer_to_output_weights_.size(); i++) {

        auto connection_weights = layer_to_output_weights_.at(i);

        for (int j = 0; j < connection_weights.size(); j++) {

            int random_value = rand() % (101 + MUTATION_CHANCE_);
            
            double this_weight = connection_weights.at(j);
            double parter_weight = partner_network.get_layer_to_output_weight(j, i);
            double new_weight = random_value > 50 ? this_weight : parter_weight;
            
            if (random_value <= MUTATION_CHANCE_) {

                int multiplier = rand() % 101;
                double amount = (rand() % 500) / 30;

                if (multiplier > 50)
                    new_weight += amount;
                else
                    new_weight -= amount;

            }

            offspring_network.set_layer_to_output_weight(j, i, new_weight);

        }

    }

    return offspring_network;

}

void GeneticNeuralNetwork::set_input_to_layer_weight(int input_index, int layer_index, double value) {

    if (input_index >= INPUT_NEURONS_)
        return;
    
    if (layer_index >= LAYER_NEURONS_)
        return;
    
    input_to_layer_weights_.at(layer_index).at(input_index) = value;

}

double GeneticNeuralNetwork::get_input_to_layer_weight(int input_index, int layer_index) {

    if (input_index >= INPUT_NEURONS_)
        return 0;
    
    if (layer_index >= LAYER_NEURONS_)
        return 0;
    
    return input_to_layer_weights_.at(layer_index).at(input_index);

}

void GeneticNeuralNetwork::set_layer_to_output_weight(int layer_index, int output_index, double value) {

    if (output_index >= OUTPUT_NEURONS_)
        return;
    
    if (layer_index >= LAYER_NEURONS_)
        return;
    
    layer_to_output_weights_.at(output_index).at(layer_index) = value;

}

double GeneticNeuralNetwork::get_layer_to_output_weight(int layer_index, int output_index) {

    if (output_index >= OUTPUT_NEURONS_)
        return 0;
    
    if (layer_index >= LAYER_NEURONS_)
        return 0;
    
    return layer_to_output_weights_.at(output_index).at(layer_index);

}

double GeneticNeuralNetwork::activation_function_(double weight_sum) {

    return 1 / (1 + std::exp(-weight_sum));

}