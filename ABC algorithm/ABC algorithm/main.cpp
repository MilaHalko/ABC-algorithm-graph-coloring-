#include "ABCAlgorithm.hpp"

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    vector<vector<bool>> graph (3);
    GraphGeneration(graph);
    ABC(graph);
    return 0;
}
