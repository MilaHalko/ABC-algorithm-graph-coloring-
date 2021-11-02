#include <iostream>
#include <vector>
#include <time.h>
using namespace::std;
const int VERTEXES = 10;
const int MIN_VERTEX = 2, MAX_VERTEX = 10;

void GraphGeneration(vector<vector<bool>> &graph) {
    graph.resize(VERTEXES);
    for (int i = 0; i < VERTEXES; i++) {graph[i].resize(VERTEXES);}
    
    srand(time(0));
    for (int i = 0; i < VERTEXES; i++) {
        int degree = 0;
        for (int j = 0; j < VERTEXES; j++) {graph[i][j] ? degree++ : degree += 0;}
        degree = rand() % (MAX_VERTEX - 1) + MIN_VERTEX - degree;
        
        for (int j = 0; j < degree; j++) {
            int randVertex;
            do {randVertex = rand() % MAX_VERTEX;} while (graph[i][randVertex]);
            graph[i][randVertex] = true;
            graph[randVertex][i] = true;
        }
    }
    
    for (auto line : graph) {
        for (auto vertex: line) {cout << vertex << "  ";}
        cout << endl;
    }
}

class Vertex {
    int color;
    int nectar;
    
public:
    Vertex() {}
    Vertex(int degree): color(0), nectar(degree) {}
    int getNectar() {return nectar;}
};

void ABC(vector<vector<bool>> graph) {
    vector<Vertex> vertexes (VERTEXES);
    for (int i = 0; i < VERTEXES; i++) {
        int degree = 0;
        for (int j = 0; j < VERTEXES; j++) {{graph[i][j] ? degree++ : degree += 0;}}
        vertexes[i] = Vertex(degree);
    }
}

int main() {
    vector<vector<bool>> graph;
    GraphGeneration(graph);
    ABC(graph);
    return 0;
}
