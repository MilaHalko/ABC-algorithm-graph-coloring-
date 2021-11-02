#include <iostream>
#include <vector>
#include <time.h>
using namespace::std;
const int VERTEXES = 10;
const int MIN_VERTEX = 2, MAX_VERTEX = 10;
const int ITERATIONS = 1;
const int ONLOOKERBEES = 32;
int EMPLOYEDBEES = 3;

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
    //MILA
    for (auto line : graph) {
        for (auto vertex: line) {cout << vertex << "  ";}
        cout << endl;
    }
    cout << endl;
}

class Vertex {
    int color;
    int nectar;
    
public:
    Vertex() {}
    Vertex(int degree): color(0), nectar(degree) {}
    int getNectar() {return nectar;}
};

int coloring(vector<Vertex> vertexes, vector<vector<bool>> graph) {
    int colorNums = 0;
    srand(time(0));
    vector<int> employedVertexes (0);
    
    // FIRST STEP (EMPLOYED BEES) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // find vertex for all employed bees
    for (int i = 0; i < EMPLOYEDBEES; i++) {
        int v; // vertex to add to employedVertexes
        
        // check if vertex is already added to employedVertexes
        bool alreadyAdded = false;
        do {
            alreadyAdded = false;
            v = rand() % VERTEXES;
            for (auto elem: employedVertexes) {
                if (elem == v) {
                    alreadyAdded = true;
                    break;
                }
            }
        } while (alreadyAdded);
        employedVertexes.push_back(v);
    }
    
    // MILA
    cout << "Employed vertexes: ";
    for (auto elem: employedVertexes) {cout << elem << "  ";}
    cout << endl << endl;
    
    // SECOND STEP (ONLOOKER BEES' ALLOCATION) ~~~~~~~~~~~~~~~~~~~~
    // get amount of nectar
    int allNectar = 0;
    for (int i = 0; i < employedVertexes.size(); i++) {
        allNectar += vertexes[employedVertexes[i]].getNectar();
    }
    //MILA
    cout << "All nectar: " << allNectar << endl << endl;
    
    // bees' proportion
    vector<int> beesProportion (0);
    int startPoint = 0;
    for (int v = 0; v < employedVertexes.size(); v++) {
        int nectar = vertexes[employedVertexes[v]].getNectar();
        for (int i = startPoint; i < nectar + startPoint; i++) {beesProportion.push_back(v);}
    }
    //MILA
    cout << "Bees' proportion:\n";
    for (auto elem: beesProportion) {cout << elem << " ";}
    cout << endl << endl;
    
    // bees' allocation
    vector<int> onlookersOnVertex (EMPLOYEDBEES);
    for (int bee = 0; bee < ONLOOKERBEES; bee++) {
        int percent = rand() % allNectar;
        onlookersOnVertex[beesProportion[percent]]++; // checking proportion range to get vertex number
    }
    
    //MILA
    cout << "Bees num on every vertex:\n";
    for (auto elem: onlookersOnVertex) {cout << elem << "  ";}
    cout << endl << endl;
    
    return colorNums;
}

void ABC(vector<vector<bool>> graph) {
    // creating vertexes with it's degree info
    vector<Vertex> vertexes (VERTEXES);
    for (int i = 0; i < VERTEXES; i++) {
        int degree = 0;
        for (int j = 0; j < VERTEXES; j++) {{graph[i][j] ? degree++ : degree += 0;}}
        vertexes[i] = Vertex(degree);
    }
    
    // iteration process
    int chromaticNum = coloring(vertexes, graph);
    int bestChromaticNum = chromaticNum;
    for (int i = 2; i <= ITERATIONS; i++) {
        chromaticNum = coloring(vertexes, graph);
        bestChromaticNum = bestChromaticNum > chromaticNum ? chromaticNum : bestChromaticNum;
        if (i % 20 == 0) {cout << i <<") Best chromatic number: " << bestChromaticNum << endl;}
    }
}

int main() {
    vector<vector<bool>> graph;
    GraphGeneration(graph);
    ABC(graph);
    return 0;
}
