//
// Created by Colin Wishart on 11/27/2023.
//

#ifndef PROJECT3_ADJLIST_H
#define PROJECT3_ADJLIST_H


#include <map>
#include <unordered_map>
#include <vector>


using namespace std;

class AdjList {

    int numNodes; // total number of nodes for calculates

private:

    map<int, vector<pair<int, int>>> adjList;

public:

    AdjList(); // assigns member variables default values
    void insertEdge(const int& from, const int& to, const int& weight); // adds a new link between two nodes
    void printGraph();
    void numEdges();

};


#endif //PROJECT3_ADJLIST_H
