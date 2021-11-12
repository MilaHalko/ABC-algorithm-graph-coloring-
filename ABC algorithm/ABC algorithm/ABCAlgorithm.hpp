#pragma once
#include "libs.hpp"

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


//Oh no! Where are OOP principles?
void GraphGeneration(vector<vector<bool>> &graph);

//ABC algorithm
void ABC(vector<vector<bool>> graph);
int coloring(vector<Vertex> vertexes, vector<vector<bool>> graph);
//main steps in ABC
void setEmployedVertexes(vector<int> &employedVertexes, vector<Vertex> vertexes);
vector<int> onlookersAllocation(vector<int> employedVertexes, vector<Vertex> vertexes);
void onlookersColoring(vector<int> employedVertexes, vector<Vertex> &vertexes, vector<vector<bool>> graph, vector<int> onlookersOnVertex);
//additional functions
int getChromaticNumber(vector<Vertex> vertexes);
bool graphColored(vector<Vertex> vertexes);
void recountNectar(int vertex, vector<vector<bool>> graph, vector<Vertex> &vertexes);

