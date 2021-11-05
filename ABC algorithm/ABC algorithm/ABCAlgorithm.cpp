#include "ABCAlgorithm.hpp"

const int VERTEXES = 250;
const int MIN_VERTEX = 2, MAX_VERTEX = 25;
const int ITERATIONS = 1000;
      int EMPLOYEDBEES;      // get info about nectar (chose vertexes)
const int CONST_EMPLOYEDBEES = 3;
const int ONLOOKERBEES = 32;  // pick nectar (color vertexes)

void GraphGeneration(vector<vector<bool>> &graph) {
    graph.resize(VERTEXES);
    for (int i = 0; i < VERTEXES; i++) {graph[i].resize(VERTEXES);}
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
    /*
    for (auto line : graph) {
        for (auto vertex: line) {cout << vertex << "  ";}
        cout << endl;
    }
    cout << endl; */
}


// ITERATION PROCCESS
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
    cout << "1) First chromatic number: " << chromaticNum << endl;
    //cout << "1) Chromatic number: " << chromaticNum << endl << endl;
    int bestChromaticNum = chromaticNum;
    for (int i = 2; i <= ITERATIONS; i++) {
        chromaticNum = coloring(vertexes, graph);
        bestChromaticNum = bestChromaticNum > chromaticNum ? chromaticNum : bestChromaticNum;
        if (i % 20 == 0) {cout << i <<") Best chromatic number: " << bestChromaticNum << endl;}
        //cout << i <<") Chromatic number: " << chromaticNum << endl << endl;
    }
}

// MAIN PROCCESS
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
    
    //COLORS COUT (OPTIONAL)
    /*
    cout << "Colors: ";
    for (auto v: vertexes) {cout << v.getColor() << " ";}
    cout << endl; */
    
    return chromaticNumber;
}

// FIRST STEP  -  SET ALL EMPLOYED BEES ON VERTEXES
void setEmployedVertexes(vector<int> &employedVertexes, vector<Vertex> vertexes) {
    // find vertex for all employed bees
    int hasNectar = 0;
    for (auto v: vertexes) {if (v.getNectar()) hasNectar++;}
    int enoughBees = EMPLOYEDBEES - hasNectar >= 0 ? hasNectar : EMPLOYEDBEES;
    
    for (int i = 0; i < enoughBees; i++) {
        int v;
        // check if vertex is already added to employedVertexes  or  v's nectar == 0
        bool alreadyAdded = false;
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
    /*
    cout << "Employed vertexes: ";
    for (auto elem: employedVertexes) {cout << elem << "  ";}
    cout << endl; */
}

// SECOND STEP  -  ALLOCATE ALL ONLOOKER BEES ON CHOSEN VERTEXES
vector<int> onlookersAllocation(vector<int> employedVertexes, vector<Vertex> vertexes) {
    int allNectar = 0;
    for (int i = 0; i < employedVertexes.size(); i++) {
        allNectar += vertexes[employedVertexes[i]].getNectar();
    }
    vector<int> beesProportion (0);
    int startPoint = 0;
    for (int v = 0; v < employedVertexes.size(); v++) {
        int nectar = vertexes[employedVertexes[v]].getNectar();
        for (int i = startPoint; i < nectar + startPoint; i++) {beesProportion.push_back(v);}
    }
    
    // bees' allocation
    vector<int> onlookersOnVertex (EMPLOYEDBEES);
    if (ONLOOKERBEES - allNectar < 0) {
        for (int bee = 0; bee < ONLOOKERBEES; bee++) {
            do {
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
    
    //(OPTIONAL)
    /*
    cout << "Bees num in every vertex: ";
    for (auto elem: onlookersOnVertex) {cout << elem << "  ";}
    cout << endl; */
    
    return onlookersOnVertex;
}

// THIRD STEP  -  COLOR ADJACENT VERTEXES BY ONLOOKER BEES
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

// ADDITIONAL FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int getChromaticNumber(vector<Vertex> vertexes) {
    int chromaticNumber = 2;
    for (auto vertex: vertexes) {if (chromaticNumber < vertex.getColor()) chromaticNumber = vertex.getColor();}
    return chromaticNumber;
}

bool graphColored(vector<Vertex> vertexes) {
    for (auto vertex: vertexes) { if(vertex.getColor() == 0) return false;}
    return true;
}

void recountNectar(int vertex, vector<vector<bool>> graph, vector<Vertex> &vertexes) {
    int nectar = 0;
    for (int adjacent = 0; adjacent < VERTEXES; adjacent++) {
        if (graph[vertex][adjacent] && vertexes[adjacent].getColor() == 0) {
            nectar++;
        }
    }
    vertexes[vertex].setNectar(nectar);
}
