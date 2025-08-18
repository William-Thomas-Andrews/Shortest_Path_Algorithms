#include "GNN.hpp"

GNN::GNN() {}

GNN::GNN(Graph G) {}

void GNN::train() { // Try with single instance input for each iteration


    // Loop begins, for n iterations:

        // Input vertices from Graph G into the input_vertices matrix (perhaps just two input vertices, the beginning and the end?)

        // Weight and bias matrix multiplication from each perceptron and activation function, then feeds to the ouptut stage.

        // Calculate the Dijkstra actual path and put in a cost matrix by entering in the weights

        // Calculate result with cost function matrix by subtracting from the weight matrix chosen by the model

        // Perform the rest of back prop according with the sheet I wrote

        // Update the parameters

        // Repeat loop ~

    // Return;

}

// Possibly change return type later
std::vector<Edge> GNN::predict(Vertex start, Vertex end) { 

    // Perform forward prop from above

    // Return prediction vector (or other way of showing the path taken);

}