#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
using namespace::std;
const int VERTEXES = 5;
const int MIN_VERTEX = 2, MAX_VERTEX = 5;
const int ITERATIONS = 5;
      int EMPLOYEDBEES;      // get info about nectar (chose vertexes)
const int CONST_EMPLOYEDBEES = 2;
const int ONLOOKERBEES = 5;  // pick nectar (color vertexes)

void GraphGeneration(vector<vector<bool>> &graph) {
    graph.resize(VERTEXES);
    for (int i = 0; i < VERTEXES; i++) {graph[i].resize(VERTEXES);}

    srand(time(NULL));
    for (int i = 0; i < VERTEXES; i++) {
        int degree = 0;
        for (int j = 0; j < VERTEXES; j++) {graph[i][j] ? degree++ : degree += 0;}
        degree = rand() % (MAX_VERTEX - 1) + MIN_VERTEX - degree;
        
        for (int j = 0; j < degree; j++) {
            int randVertex;
            do {randVertex = rand() % VERTEXES;} while (i == randVertex);
            graph[i][randVertex] = true;
            graph[randVertex][i] = true;
        }
    }
    //GRAPH COUT (OPTIONAL)
    //
    for (auto line : graph) {
        for (auto vertex: line) {cout << vertex << "  ";}
        cout << endl;
    }
    cout << endl;
     //
}

class Vertex {
    int color;
    int nectar;
    
public:
    Vertex() {}
    Vertex(int degree): color(0), nectar(degree) {}
    int getNectar() {return nectar;}
    int getColor() {return color;}
    void setColor(int color) {this->color = color;}
    void setNectar(int nectar) {this->nectar = nectar;}
};

void setEmployedVertexes(vector<int> &employedVertexes, vector<Vertex> vertexes) {
    // find vertex for all employed bees
    int hasNectar = 0;
    for (auto v: vertexes) {if (v.getNectar()) hasNectar++;}
    int enoughBees = EMPLOYEDBEES - hasNectar >= 0 ? hasNectar : EMPLOYEDBEES;
    
    for (int i = 0; i < enoughBees; i++) {
        int v;
        // check if vertex is already added to employedVertexes  or  v's nectar == 0
        bool alreadyAdded = false;
        srand(time(nullptr));
        do {
            alreadyAdded = false;
            v = rand() % VERTEXES;
            for (auto elem: employedVertexes) {
                if (elem == v || vertexes[v].getNectar() == 0) {
                    alreadyAdded = true;
                    break;
                }
            }
        } while (alreadyAdded);
        employedVertexes.push_back(v);
    }
    //EMPLOYED VERTEXES COUT (OPTIONAL)
    //
    cout << "Employed vertexes: ";
    for (auto elem: employedVertexes) {cout << elem << "  ";}
    cout << endl;
    //
}

vector<int> onlookersAllocation(vector<int> employedVertexes, vector<Vertex> vertexes) {
    // get amount of nectar
    int allNectar = 0;
    for (int i = 0; i < employedVertexes.size(); i++) {
        allNectar += vertexes[employedVertexes[i]].getNectar();
    }
    //ALL NECTAR COUT (OPTIONAL)
    //cout << "All nectar: " << allNectar << endl << endl;
    
    // bees' proportion
    vector<int> beesProportion (0);
    int startPoint = 0;
    for (int v = 0; v < employedVertexes.size(); v++) {
        int nectar = vertexes[employedVertexes[v]].getNectar();
        for (int i = startPoint; i < nectar + startPoint; i++) {beesProportion.push_back(v);}
    }
    //BEES PROPORTION COUT (OPTIONAL)
    /*
    cout << "Bees' proportion:\n";
    for (auto elem: beesProportion) {cout << elem << " ";}
    cout << endl << endl; */
    
    // bees' allocation
    vector<int> onlookersOnVertex (EMPLOYEDBEES);
    if (ONLOOKERBEES - allNectar < 0) {
        for (int bee = 0; bee < ONLOOKERBEES; bee++) {
            do {
                srand(time(0));
                int pos = rand() % allNectar;
                if (beesProportion[pos] != -1) {
                    onlookersOnVertex[beesProportion[pos]]++; // checking proportion range to get vertex number
                    beesProportion[pos] = -1;
                    break;
                }
            } while (true);
        }
    }
    else {
        for (int v = 0; v < employedVertexes.size(); v++) {
            int nectar = vertexes[employedVertexes[v]].getNectar();
            onlookersOnVertex[v] = nectar;
        }
    }
    
    //BEES' NUMBER IN EVERY VERTEX COUT (OPTIONAL)
    //
    cout << "Bees num in every vertex: ";
    for (auto elem: onlookersOnVertex) {cout << elem << "  ";}
    cout << endl;
    //
    
    return onlookersOnVertex;
}

void recountNectar(int vertex, vector<vector<bool>> graph, vector<Vertex> &vertexes) {
    int nectar = 0;
    for (int adjacent = 0; adjacent < VERTEXES; adjacent++) {
        if (graph[vertex][adjacent] && vertexes[adjacent].getColor() == 0) {
            nectar++;
        }
    }
    vertexes[vertex].setNectar(nectar);
    
    //NEW NECTAR NUMBER COUT (OPTIONAL)
    //cout << "New nectar: " << nectar << endl << endl;
}

void onlookersColoring(vector<int> employedVertexes, vector<Vertex> &vertexes, vector<vector<bool>> graph, vector<int> onlookersOnVertex) {
    for (int v = 0; v < employedVertexes.size(); v++) {
        int vertex = employedVertexes[v];
        int onlookersCount = 0;
        
        // firstly check uncolored vertexes ..
        for (int adjacent = 0; adjacent < VERTEXES; adjacent++) {
            if (graph[vertex][adjacent] && vertexes[adjacent].getColor() == 0) {
                int color = 1;
                for (int i = 0; i < VERTEXES; i++) {
                    if (graph[adjacent][i] && vertexes[i].getColor() == color) {
                        i = -1;
                        color++;
                    }
                }
                vertexes[adjacent].setColor(color);
                onlookersCount++;
                if (onlookersCount == onlookersOnVertex[v]) {break;}
            }
        }
        
        // ..secondly check colored vertexes
        if (onlookersCount < onlookersOnVertex[v]) {
            vector<int> checkedVertexes (0);
            int adjacent;
            do {
                srand(time(0));
                do {adjacent = rand() % VERTEXES;} while (graph[v][adjacent] == 0);
                int color = 1;
                for (int i = 0; i < VERTEXES; i++) {
                    if (graph[adjacent][i] && vertexes[i].getColor() == color) {
                        i = -1;
                        color++;
                    }
                }
                vertexes[adjacent].setColor(color);
                onlookersCount++;
            } while (onlookersCount != onlookersOnVertex[v]);
        }
        recountNectar(vertex, graph, vertexes);
        if (vertexes[vertex].getNectar() == 0) {
            int color = 1;
            for (int adjacent = 0; adjacent < VERTEXES; adjacent++) {
                if (graph[vertex][adjacent] && vertexes[adjacent].getColor() == color) {
                    adjacent = -1;
                    color++;
                }
            }
            vertexes[vertex].setColor(color);
            EMPLOYEDBEES++;
        }
    }
}

bool graphColored(vector<Vertex> vertexes) {
    for (auto vertex: vertexes) { if(vertex.getColor() == 0) return false;}
    return true;
}

int getChromaticNumber(vector<Vertex> vertexes) {
    int chromaticNumber = 2;
    for (auto vertex: vertexes) {if (chromaticNumber < vertex.getColor()) chromaticNumber = vertex.getColor();}
    return chromaticNumber;
}

int coloring(vector<Vertex> vertexes, vector<vector<bool>> graph) {
    do {
        EMPLOYEDBEES = CONST_EMPLOYEDBEES;
        // FIRST STEP (EMPLOYED BEES) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        vector<int> employedVertexes (0);
        setEmployedVertexes(employedVertexes, vertexes);
        
        // SECOND STEP (ONLOOKER BEES' ALLOCATION) ~~~~~~~~~~~~~~~~~~~~
        vector<int> onlookersOnVertex = onlookersAllocation(employedVertexes, vertexes);
        
        // THIRD STEP (ONLOOKER BEES' COLORING) ~~~~~~~~~~~~~~~~~~~~~~~
        onlookersColoring(employedVertexes, vertexes, graph, onlookersOnVertex);
    } while (!graphColored(vertexes));
    
    int chromaticNumber = getChromaticNumber(vertexes);
    cout << "Chromatic number: " << chromaticNumber << endl;
    
    //COLORS COUT (OPTIONAL)
    cout << "Colors: ";
    for (auto v: vertexes) {cout << v.getColor() << " ";}
    cout << endl << endl;
    
    return chromaticNumber;
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
    //cout << "1) Chromatic number: " << chromaticNum << endl;
    int bestChromaticNum = chromaticNum;
    for (int i = 2; i <= ITERATIONS; i++) {
        chromaticNum = coloring(vertexes, graph);
        bestChromaticNum = bestChromaticNum > chromaticNum ? chromaticNum : bestChromaticNum;
        //if (i % 20 == 0) {cout << i <<") Best chromatic number: " << bestChromaticNum << endl;}
    }
}

int main() {
    vector<vector<bool>> graph;
    GraphGeneration(graph);
    ABC(graph);
    return 0;
}
